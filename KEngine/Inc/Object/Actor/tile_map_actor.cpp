#include "KEngine.h"
#include "tile_map_actor.h"

#include "device_manager.h"
#include "Resource/resource_manager.h"
#include "Resource/mesh.h"
#include "Rendering/rendering_manager.h"
#include "Rendering/shader.h"
#include "Rendering/render_state.h"
#include "World/world_manager.h"
#include "Object/object_manager.h"
#include "Object/Actor/camera_actor.h"
#include "Object/Actor/tile_actor.h"
#include "Object/Component/transform.h"
#include "Object/Component/material.h"

void K::TileMapActor::Initialize()
{
	try
	{
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "TileMapActor::Initialize" << std::endl;
	}
}

K::APTR K::TileMapActor::Clone() const
{
	return APTR{ new TileMapActor{ *this }, [](TileMapActor* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::TileMapActor::Serialize(InputMemoryStream& _imstream)
{
	_imstream.Serialize(type_);

	_imstream.Serialize(tile_size_.x);
	_imstream.Serialize(tile_size_.y);

	_imstream.Serialize(tile_map_size_.x);
	_imstream.Serialize(tile_map_size_.y);

	tile_map_.clear();
	tile_graph_.clear();

	size_t y_count{};
	_imstream.Serialize(y_count);
	tile_map_.resize(y_count);
	tile_graph_.resize(y_count);

	for (auto i = 0; i < y_count; ++i)
	{
		size_t x_count{};
		_imstream.Serialize(x_count);
		tile_map_.at(i).resize(x_count);
		tile_graph_.at(i).resize(x_count);

		for (auto j = 0; j < x_count; ++j)
			tile_map_.at(i).at(j)->Serialize(_imstream);
	}

	_CreateGraph();
}

void K::TileMapActor::Serialize(OutputMemoryStream& _omstream)
{
	_omstream.Serialize(type_);

	_omstream.Serialize(tile_size_.x);
	_omstream.Serialize(tile_size_.y);

	_omstream.Serialize(tile_map_size_.x);
	_omstream.Serialize(tile_map_size_.y);

	auto y_count = tile_map_.size();
	_omstream.Serialize(y_count);

	for (auto i = 0; i < y_count; ++i)
	{
		auto x_count = tile_map_.at(i).size();
		_omstream.Serialize(x_count);

		for (auto j = 0; j < x_count; ++j)
			tile_map_.at(i).at(j)->Serialize(_omstream);
	}
}

void K::TileMapActor::CreateMap(TILE_TYPE _type, int _x_count, int _y_count, Vector2 const& _tile_size)
{
	type_ = _type;

	tile_map_.resize(_y_count);
	tile_graph_.resize(_y_count);

	for (auto i = 0; i < _y_count; ++i)
	{
		tile_map_.at(i).resize(_x_count);
		tile_graph_.at(i).resize(_x_count);
	}

	tile_size_ = _tile_size;
	tile_map_size_ = _tile_size * Vector2{ static_cast<float>(_x_count), static_cast<float>(_y_count) };

	switch (_type)
	{
	default:
		_CreateIsometricMap();
		break;

	case TILE_TYPE::ORTHOGRAPHIC:
		_CreateOrthographicMap();
		break;
	}

	_CreateGraph();
}

K::TILE_OPTION K::TileMapActor::GetTileOption(int _x_idx, int _y_idx) const
{
	if (_x_idx < 0 || _x_idx >= tile_map_.at(0).size())
		return TILE_OPTION::MAX;

	if (_y_idx < 0 || _y_idx >= tile_map_.size())
		return TILE_OPTION::MAX;

	return tile_map_[_y_idx][_x_idx]->option();
}

K::Vector3 K::TileMapActor::GetTilePosition(std::pair<int, int> const& _idx) const
{
	return CPTR_CAST<Transform>(tile_map_.at(_idx.second).at(_idx.first)->FindComponent(TAG{ TRANSFORM, 0 }))->world().Translation();
}

std::pair<int, int> K::TileMapActor::GetTileIndex(Vector3 const& _position) const
{
	switch (type_)
	{
	default:
		return _GetIsometricTileIndex(_position);

	case TILE_TYPE::ORTHOGRAPHIC:
		return _GetOrthographicTileIndex(_position);
	}
}

void K::TileMapActor::SetTileOption(int _x_idx, int _y_idx, TILE_OPTION _option)
{
	if (_x_idx < 0 || _x_idx >= tile_map_.at(0).size())
		return;

	if (_y_idx < 0 || _y_idx >= tile_map_.size())
		return;

	tile_map_[_y_idx][_x_idx]->set_option(_option);
}

K::TileMapActor::TileMapActor(TileMapActor const& _other) : Actor(_other)
{
	type_ = _other.type_;
	tile_size_ = _other.tile_size_;
	tile_map_size_ = _other.tile_map_size_;

	tile_map_.clear();
	tile_map_.resize(_other.tile_map_.size());
	for (auto i = 0; i < _other.tile_map_.size(); ++i)
	{
		tile_map_.at(i).resize(_other.tile_map_.at(i).size());

		for (auto j = 0; j < _other.tile_map_.at(i).size(); ++j)
			tile_map_.at(i).at(j) = std::static_pointer_cast<TileActor>(_other.tile_map_.at(i).at(j)->Clone());
	}

	tile_graph_ = _other.tile_graph_;
}

K::TileMapActor::TileMapActor(TileMapActor&& _other) noexcept : Actor(std::move(_other))
{
	type_ = std::move(_other.type_);
	tile_size_ = std::move(_other.tile_size_);
	tile_map_size_ = std::move(_other.tile_map_size_);
	tile_map_ = std::move(_other.tile_map_);
	tile_graph_ = std::move(_other.tile_graph_);
}

void K::TileMapActor::_Finalize()
{
}

void K::TileMapActor::_Render(float _time)
{
	auto camera_position = CPTR_CAST<Transform>(WorldManager::singleton()->FindCamera(TAG{ DEFAULT_CAMERA, 0 })->FindComponent(TAG{ TRANSFORM, 0 }))->world().Translation();

	auto resolution = Vector3{ static_cast<float>(RESOLUTION::WIDTH), static_cast<float>(RESOLUTION::HEIGHT), 0.f };

	auto camera_view_min = camera_position - resolution * 0.5f;
	auto camera_view_max = camera_position + resolution * 0.5f;

	std::pair<int, int> x_idx_range{};
	std::pair<int, int> y_idx_range{};

	switch (type_)
	{
	default:
	{
		auto LB = _GetIsometricTileIndex(camera_view_min);
		auto LT = _GetIsometricTileIndex(camera_view_min + Vector3{ 0.f, resolution.y, 0.f });
		auto RT = _GetIsometricTileIndex(camera_view_max);
		auto RB = _GetIsometricTileIndex(camera_view_min + Vector3{ resolution.x, 0.f, 0.f });

		x_idx_range = { LB.first, RT.first };
		y_idx_range = { RB.second, LT.second };
		break;
	}

	case TILE_TYPE::ORTHOGRAPHIC:
		x_idx_range = _GetOrthographicTileIndex(camera_view_min);
		y_idx_range = _GetOrthographicTileIndex(camera_view_max);
		break;
	}

	std::vector<Matrix> matrix_vector{};

	for (auto i = y_idx_range.first; i <= y_idx_range.second; ++i)
	{
		if (i < 0 || i >= tile_map_.size())
			continue;

		for (auto j = x_idx_range.first; j <= x_idx_range.second; ++j)
		{
			if (j < 0 || j >= tile_map_.at(0).size())
				continue;

			matrix_vector.push_back(CPTR_CAST<Transform>(tile_map_[i][j]->FindComponent(TAG{ TRANSFORM, 0 }))->world().Transpose());
		}
	}

	auto const& resource_manager = ResourceManager::singleton();
	auto const& rendering_manager = RenderingManager::singleton();

	auto const& context = DeviceManager::singleton()->context();
	auto const& instance_buffer = resource_manager->FindMesh(INSTANCE_TEX_RECT)->GetInstanceBuffer(0);

	D3D11_MAPPED_SUBRESOURCE dms{};
	ThrowIfFailed(context->Map(instance_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &dms));

	int instance_count = static_cast<int>(matrix_vector.size());
	memcpy_s(dms.pData, sizeof(Matrix) * instance_count, matrix_vector.data(), sizeof(Matrix) * instance_count);

	context->Unmap(instance_buffer.Get(), 0);

	auto const& mesh = resource_manager->FindMesh(INSTANCE_TEX_RECT);
	auto const& shader = rendering_manager->FindShader(INSTANCE_TEX_SHADER);
	auto const& render_state = rendering_manager->FindRenderState(ALPHA_BLEND);
	auto const& material = CPTR_CAST<Material>(tile_map_.at(0).at(0)->FindComponent(TAG{ MATERIAL, 0 }));

	render_state->SetToShader();

	shader->SetToShader();

	material->SetToShader(0, 0);

	mesh->SetInstanceCount(0, instance_count);
	mesh->Render();
}

void K::TileMapActor::_CreateIsometricMap()
{
	auto tile_size_div2 = tile_size_ * 0.5f;
	auto start_position = Vector3{ tile_map_size_.x * 0.5f, tile_size_div2.y, 0.f };

	for (auto i = 0; i < tile_map_.size(); ++i)
	{
		for (auto j = 0; j < tile_map_.at(i).size(); ++j)
		{
			auto tile = ObjectManager::singleton()->CreateActor<TileActor>(TAG{ TILE, 0 });
			auto const& tile_transform = CPTR_CAST<Transform>(tile->FindComponent(TAG{ TRANSFORM, 0 }));

			tile_transform->set_local_scaling(Vector3{ tile_size_.x, tile_size_.y, 0.f });
			tile_transform->set_local_translation(Vector3{ start_position.x + tile_size_div2.x * (j - i), start_position.y + tile_size_div2.y * (j + i), 0.f });

			tile_map_.at(i).at(j) = std::move(std::static_pointer_cast<TileActor>(tile));
		}
	}
}

void K::TileMapActor::_CreateOrthographicMap()
{
	auto start_position = Vector3{ tile_size_.x * 0.5f, tile_size_.y * 0.5f, 0.f };

	for (auto i = 0; i < tile_map_.size(); ++i)
	{
		for (auto j = 0; j < tile_map_.at(i).size(); ++j)
		{
			auto tile = ObjectManager::singleton()->CreateActor<TileActor>(TAG{ TILE, 0 });
			auto const& tile_transform = CPTR_CAST<Transform>(tile->FindComponent(TAG{ TRANSFORM, 0 }));

			tile_transform->set_local_scaling(Vector3{ tile_size_.x, tile_size_.y, 0.f });
			tile_transform->set_local_translation(Vector3{ start_position.x + tile_size_.x * j, start_position.y + tile_size_.y * i, 0.f });

			tile_map_.at(i).at(j) = std::move(std::static_pointer_cast<TileActor>(tile));
		}
	}
}

void K::TileMapActor::_CreateGraph()
{
	for (auto i = 0; i < tile_graph_.size(); ++i)
	{
		for (auto j = 0; j < tile_graph_.at(i).size(); ++j)
		{
			if (TILE_OPTION::NORMAL == GetTileOption(j - 1, i - 1))
				tile_graph_[i][j].push_back({ j - 1, i - 1 });
			if (TILE_OPTION::NORMAL == GetTileOption(j, i - 1))
				tile_graph_[i][j].push_back({ j, i - 1 });
			if (TILE_OPTION::NORMAL == GetTileOption(j + 1, i - 1))
				tile_graph_[i][j].push_back({ j + 1, i - 1 });

			if (TILE_OPTION::NORMAL == GetTileOption(j - 1, i))
				tile_graph_[i][j].push_back({ j - 1, i });
			if (TILE_OPTION::NORMAL == GetTileOption(j + 1, i))
				tile_graph_[i][j].push_back({ j + 1, i });

			if (TILE_OPTION::NORMAL == GetTileOption(j - 1, i + 1))
				tile_graph_[i][j].push_back({ j - 1, i + 1 });
			if (TILE_OPTION::NORMAL == GetTileOption(j, i + 1))
				tile_graph_[i][j].push_back({ j, i + 1 });
			if (TILE_OPTION::NORMAL == GetTileOption(j + 1, i + 1))
				tile_graph_[i][j].push_back({ j + 1, i + 1 });
		}
	}
}

std::pair<int, int> K::TileMapActor::_GetIsometricTileIndex(Vector3 const& _position) const
{
	std::pair<int, int> index{};

	auto slope = tile_size_.y / tile_size_.x;

	auto y_intercept_for_x_idx = _position.y + slope * _position.x;
	auto y_intercept_for_y_idx = _position.y - slope * _position.x;

	auto tile_map_size_div2 = tile_map_size_ * 0.5f;

	index.first = static_cast<int>((y_intercept_for_x_idx - tile_map_size_div2.y) / tile_size_.y);
	index.second = static_cast<int>((y_intercept_for_y_idx + tile_map_size_div2.y) / tile_size_.y);

	return index;
}

std::pair<int, int> K::TileMapActor::_GetOrthographicTileIndex(Vector3 const& _position) const
{
	std::pair<int, int> index{};

	index.first = static_cast<int>(_position.x / tile_size_.x);
	index.second = static_cast<int>(_position.y / tile_size_.y);

	return index;
}
