#include "KEngine.h"
#include "resource_manager.h"

#include "path_manager.h"
#include "mesh.h"
#include "texture.h"
#include "sampler.h"

std::shared_ptr<K::Mesh> K::ResourceManager::mesh_dummy_{};
std::shared_ptr<K::Texture> K::ResourceManager::texture_dummy_{};
std::shared_ptr<K::Sampler> K::ResourceManager::sampler_dummy_{};
std::shared_ptr<K::ANIMATION_2D_CLIP_DESC> K::ResourceManager::animation_2d_clip_dummy_{};

void K::ResourceManager::Initialize()
{
	try
	{
#pragma region Mesh
		VertexTex tex_rect_vertices[4]{
			{ { -0.5f, -0.5f, 0.f }, { 0.f, 1.f } },
			{ { -0.5f, 0.5f, 0.f }, { 0.f, 0.f } },
			{ { 0.5f, 0.5f, 0.f }, { 1.f, 0.f } },
			{ { 0.5f, -0.5f, 0.f }, { 1.f, 1.f } }
		};

		uint16_t tex_rect_indices[6]{ 0, 1, 2, 0, 2, 3 };

		_CreateMesh(
			TEX_RECT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			tex_rect_vertices, sizeof(VertexTex), 4, D3D11_USAGE_DEFAULT,
			tex_rect_indices, sizeof(uint16_t), 6, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R16_UINT
		);

		Vector3 collider_rect_vertices[5]{
			{ -0.5f, -0.5f, 0.f },
			{ -0.5f, 0.5f, 0.f },
			{ 0.5f, 0.5f, 0.f },
			{ 0.5f, -0.5f, 0.f },
			{ -0.5f, -0.5f, 0.f }
		};

		_CreateMesh(
			COLLIDER_RECT, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
			collider_rect_vertices, sizeof(Vector3), 5, D3D11_USAGE_DEFAULT
		);

		Vector3 collider_circle_vertices[37]{};
		for (auto i = 0; i < 37; ++i)
			collider_circle_vertices[i] = { cos(DirectX::XMConvertToRadians(i * 10.f)) * 0.5f, sin(DirectX::XMConvertToRadians(i * 10.f)) * 0.5f, 0.f };

		_CreateMesh(
			COLLIDER_CIRCLE, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			collider_circle_vertices, sizeof(Vector3), 37, D3D11_USAGE_DEFAULT
		);

		VertexTex instance_tex_rect_vertices[4]{
			{ { -0.5f, -0.5f, 0.f }, { 0.f, 1.f } },
			{ { -0.5f, 0.5f, 0.f }, { 0.f, 0.f } },
			{ { 0.5f, 0.5f, 0.f }, { 1.f, 0.f } },
			{ { 0.5f, -0.5f, 0.f }, { 1.f, 1.f } }
		};

		uint16_t instance_tex_rect_indices[6]{ 0, 1, 2, 0, 2, 3 };

		MatrixTex instance_tex_rect_instances[10000]{};
		_CreateMesh(
			INSTANCE_TEX_RECT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			instance_tex_rect_vertices, sizeof(VertexTex), 4, D3D11_USAGE_DEFAULT,
			instance_tex_rect_indices, sizeof(uint16_t), 6, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R16_UINT,
			instance_tex_rect_instances, sizeof(MatrixTex), 10000, D3D11_USAGE_DYNAMIC
		);
#pragma endregion

#pragma region Texture
		_CreateTexture2D("catacomb floor", L"catacomb floor.png", TEXTURE_PATH);

		_CreateTexture2D("cow_attack1", L"cow/cow_attack1.png", TEXTURE_PATH);
		_CreateTexture2D("cow_attack2", L"cow/cow_attack2.png", TEXTURE_PATH);
		_CreateTexture2D("cow_dead", L"cow/cow_dead.png", TEXTURE_PATH);
		_CreateTexture2D("cow_death", L"cow/cow_death.png", TEXTURE_PATH);
		_CreateTexture2D("cow_get_hit", L"cow/cow_get_hit.png", TEXTURE_PATH);
		_CreateTexture2D("cow_neutral", L"cow/cow_neutral.png", TEXTURE_PATH);
		_CreateTexture2D("cow_walk", L"cow/cow_walk.png", TEXTURE_PATH);

		_CreateTexture2D("wendigo_attack1", L"wendigo/wendigo_attack1.png", TEXTURE_PATH);
		_CreateTexture2D("wendigo_attack2", L"wendigo/wendigo_attack2.png", TEXTURE_PATH);
		_CreateTexture2D("wendigo_dead", L"wendigo/wendigo_dead.png", TEXTURE_PATH);
		_CreateTexture2D("wendigo_death", L"wendigo/wendigo_death.png", TEXTURE_PATH);
		_CreateTexture2D("wendigo_neutral", L"wendigo/wendigo_neutral.png", TEXTURE_PATH);
		_CreateTexture2D("wendigo_walk", L"wendigo/wendigo_walk.png", TEXTURE_PATH);

		_CreateTexture2D("fallen_shaman_attack1", L"fallen shaman/fallen_shaman_attack1.png", TEXTURE_PATH);
		_CreateTexture2D("fallen_shaman_attack2", L"fallen shaman/fallen_shaman_attack2.png", TEXTURE_PATH);
		_CreateTexture2D("fallen_shaman_attack2(overlay)", L"fallen shaman/fallen_shaman_attack2(overlay).png", TEXTURE_PATH);
		_CreateTexture2D("fallen_shaman_dead", L"fallen shaman/fallen_shaman_dead.png", TEXTURE_PATH);
		_CreateTexture2D("fallen_shaman_death", L"fallen shaman/fallen_shaman_death.png", TEXTURE_PATH);
		_CreateTexture2D("fallen_shaman_get_hit", L"fallen shaman/fallen_shaman_get_hit.png", TEXTURE_PATH);
		_CreateTexture2D("fallen_shaman_neutral", L"fallen shaman/fallen_shaman_neutral.png", TEXTURE_PATH);
		_CreateTexture2D("fallen_shaman_walk", L"fallen shaman/fallen_shaman_walk.png", TEXTURE_PATH);

		_CreateTexture2D("andariel_attack1", L"andariel/andariel_attack1.png", TEXTURE_PATH);
		_CreateTexture2D("andariel_death", L"andariel/andariel_death.png", TEXTURE_PATH);
		_CreateTexture2D("andariel_death(overlay)", L"andariel/andariel_death(overlay).png", TEXTURE_PATH);
		_CreateTexture2D("andariel_get_hit", L"andariel/andariel_get_hit.png", TEXTURE_PATH);
		_CreateTexture2D("andariel_neutral", L"andariel/andariel_neutral.png", TEXTURE_PATH);
		_CreateTexture2D("andariel_walk", L"andariel/andariel_walk.png", TEXTURE_PATH);
		_CreateTexture2D("andariel_special_cast", L"andariel/andariel_special_cast.png", TEXTURE_PATH);
		_CreateTexture2D("andariel_special_cast(overlay)", L"andariel/andariel_special_cast(overlay).png", TEXTURE_PATH);
#pragma endregion

#pragma region Sampler
		_CreateSampler(
			LINEAR_SAMPLER,
			D3D11_FILTER_MIN_MAG_MIP_LINEAR,
			D3D11_TEXTURE_ADDRESS_CLAMP,
			D3D11_TEXTURE_ADDRESS_CLAMP,
			D3D11_TEXTURE_ADDRESS_CLAMP
		);
#pragma endregion

#pragma region Animation2DClip
		_CreateAnimation2DClip("cow_attack1", L"cow_attack1.csv", DATA_PATH);
		_CreateAnimation2DClip("cow_attack2", L"cow_attack2.csv", DATA_PATH);
		_CreateAnimation2DClip("cow_dead", L"cow_dead.csv", DATA_PATH);
		_CreateAnimation2DClip("cow_death", L"cow_death.csv", DATA_PATH);
		_CreateAnimation2DClip("cow_get_hit", L"cow_get_hit.csv", DATA_PATH);
		_CreateAnimation2DClip("cow_neutral", L"cow_neutral.csv", DATA_PATH);
		_CreateAnimation2DClip("cow_walk", L"cow_walk.csv", DATA_PATH);

		_CreateAnimation2DClip("wendigo_attack1", L"wendigo_attack1.csv", DATA_PATH);
		_CreateAnimation2DClip("wendigo_attack2", L"wendigo_attack2.csv", DATA_PATH);
		_CreateAnimation2DClip("wendigo_dead", L"wendigo_dead.csv", DATA_PATH);
		_CreateAnimation2DClip("wendigo_death", L"wendigo_death.csv", DATA_PATH);
		_CreateAnimation2DClip("wendigo_neutral", L"wendigo_neutral.csv", DATA_PATH);
		_CreateAnimation2DClip("wendigo_walk", L"wendigo_walk.csv", DATA_PATH);

		_CreateAnimation2DClip("fallen_shaman_attack1", L"fallen_shaman_attack1.csv", DATA_PATH);
		_CreateAnimation2DClip("fallen_shaman_attack2", L"fallen_shaman_attack2.csv", DATA_PATH);
		_CreateAnimation2DClip("fallen_shaman_attack2(overlay)", L"fallen_shaman_attack2(overlay).csv", DATA_PATH);
		_CreateAnimation2DClip("fallen_shaman_dead", L"fallen_shaman_dead.csv", DATA_PATH);
		_CreateAnimation2DClip("fallen_shaman_death", L"fallen_shaman_death.csv", DATA_PATH);
		_CreateAnimation2DClip("fallen_shaman_get_hit", L"fallen_shaman_get_hit.csv", DATA_PATH);
		_CreateAnimation2DClip("fallen_shaman_neutral", L"fallen_shaman_neutral.csv", DATA_PATH);
		_CreateAnimation2DClip("fallen_shaman_walk", L"fallen_shaman_walk.csv", DATA_PATH);

		_CreateAnimation2DClip("andariel_attack1", L"andariel_attack1.csv", DATA_PATH);
		_CreateAnimation2DClip("andariel_death", L"andariel_death.csv", DATA_PATH);
		_CreateAnimation2DClip("andariel_death(overlay)", L"andariel_death(overlay).csv", DATA_PATH);
		_CreateAnimation2DClip("andariel_get_hit", L"andariel_get_hit.csv", DATA_PATH);
		_CreateAnimation2DClip("andariel_neutral", L"andariel_neutral.csv", DATA_PATH);
		_CreateAnimation2DClip("andariel_walk", L"andariel_walk.csv", DATA_PATH);
		_CreateAnimation2DClip("andariel_special_cast", L"andariel_special_cast.csv", DATA_PATH);
		_CreateAnimation2DClip("andariel_special_cast(overlay)", L"andariel_special_cast(overlay).csv", DATA_PATH);
#pragma endregion
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "ResourceManager::Initialize" << std::endl;
	}
}

std::shared_ptr<K::Mesh> const& K::ResourceManager::FindMesh(std::string const& _tag) const
{
	auto iter = mesh_map_.find(_tag);

	if (iter == mesh_map_.end())
		return mesh_dummy_;

	return iter->second;
}

std::shared_ptr<K::Texture> const& K::ResourceManager::FindTexture(std::string const& _tag) const
{
	auto iter = texture_map_.find(_tag);

	if (iter == texture_map_.end())
		return texture_dummy_;

	return iter->second;
}

std::shared_ptr<K::Sampler> const& K::ResourceManager::FindSampler(std::string const& _tag) const
{
	auto iter = sampler_map_.find(_tag);

	if (iter == sampler_map_.end())
		return sampler_dummy_;

	return iter->second;
}

std::shared_ptr<K::ANIMATION_2D_CLIP_DESC> const& K::ResourceManager::FindAnimation2DClip(std::string const& _tag) const
{
	auto iter = animation_2d_clip_map_.find(_tag);

	if (iter == animation_2d_clip_map_.end())
		return animation_2d_clip_dummy_;

	return iter->second;
}

void K::ResourceManager::_Finalize()
{
}

void K::ResourceManager::_CreateMesh(
	std::string const& _tag, D3D11_PRIMITIVE_TOPOLOGY _topology,
	void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage)
{
	if (FindMesh(_tag))
		throw std::exception{ "ResourceManager::_CreateMesh" };

	auto mesh = std::shared_ptr<Mesh>{ new Mesh, [](Mesh* _p) {
		delete _p;
	} };

	mesh->_CreateMesh(_topology, _vtx_data, _vtx_stride, _vtx_count, _vtx_usage);

	mesh_map_.insert(std::make_pair(_tag, std::move(mesh)));
}

void K::ResourceManager::_CreateMesh(
	std::string const& _tag, D3D11_PRIMITIVE_TOPOLOGY _topology,
	void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage,
	void* _idx_data, int _idx_stride, int _idx_count, D3D11_USAGE _idx_usage, DXGI_FORMAT _idx_format)
{
	if (FindMesh(_tag))
		throw std::exception{ "ResourceManager::_CreateMesh" };

	auto mesh = std::shared_ptr<Mesh>{ new Mesh, [](Mesh* _p) {
		delete _p;
	} };

	mesh->_CreateMesh(_topology, _vtx_data, _vtx_stride, _vtx_count, _vtx_usage, _idx_data, _idx_stride, _idx_count, _idx_usage, _idx_format);

	mesh_map_.insert(std::make_pair(_tag, std::move(mesh)));
}

void K::ResourceManager::_CreateMesh(
	std::string const& _tag, D3D11_PRIMITIVE_TOPOLOGY _topology,
	void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage,
	void* _idx_data, int _idx_stride, int _idx_count, D3D11_USAGE _idx_usage, DXGI_FORMAT _idx_format,
	void* _inst_data, int _inst_stride, int _inst_count, D3D11_USAGE _inst_usage)
{
	if(FindMesh(_tag))
		throw std::exception{ "ResourceManager::_CreateMesh" };

	auto mesh = std::shared_ptr<Mesh>{ new Mesh, [](Mesh* _p) {
		delete _p;
	} };

	mesh->_CreateMesh(_topology, _vtx_data, _vtx_stride, _vtx_count, _vtx_usage, _idx_data, _idx_stride, _idx_count, _idx_usage, _idx_format, _inst_data, _inst_stride, _inst_count, _inst_usage);

	mesh_map_.insert(std::make_pair(_tag, std::move(mesh)));
}

void K::ResourceManager::_CreateTexture2D(std::string const& _tag, std::wstring const& _file_name, std::string const& _path_tag)
{
	if (FindTexture(_tag))
		throw std::exception{ "ResourceManager::_CreateTexture2D" };

	auto texture = std::shared_ptr<Texture>{ new Texture, [](Texture* _p) {
		delete _p;
	} };

	texture->_CreateTexture2D(_file_name, _path_tag);

	texture_map_.insert(std::make_pair(_tag, std::move(texture)));
}

void K::ResourceManager::_CreateSampler(
	std::string const& _tag,
	D3D11_FILTER _filter,
	D3D11_TEXTURE_ADDRESS_MODE _address_u,
	D3D11_TEXTURE_ADDRESS_MODE _address_v,
	D3D11_TEXTURE_ADDRESS_MODE _address_w)
{
	if (FindSampler(_tag))
		throw std::exception{ "ResourceManager::_CreateSampler" };

	auto sampler = std::shared_ptr<Sampler>{ new Sampler, [](Sampler* _p) {
		delete _p;
	} };

	sampler->_CreateSampler(_filter, _address_u, _address_v, _address_w);

	sampler_map_.insert(std::make_pair(_tag, std::move(sampler)));
}

void K::ResourceManager::_CreateAnimation2DClip(std::string const& _tag, std::wstring const& _file_name, std::string const& _path_tag)
{
	std::filesystem::path path_buffer = PathManager::singleton()->FindPath(_path_tag);

	if (path_buffer.empty())
		throw std::exception{ "ResourceManager::_CreateAnimation2DClip" };

	path_buffer /= _file_name;

	std::ifstream file{ path_buffer };

	if(file.fail())
		throw std::exception{ "ResourceManager::_CreateAnimation2DClip" };

	std::string line{};
	std::stringstream line_stream{};

	std::getline(file, line);
	std::getline(file, line);
	line_stream.str(line);

	std::string clip_tag{};
	std::string texture_tag{};
	std::string type{};
	std::string option{};
	std::string width{};
	std::string height{};
	std::string completion_time{};

	std::getline(line_stream, clip_tag, ',');
	std::getline(line_stream, texture_tag, ',');
	std::getline(line_stream, type, ',');
	std::getline(line_stream, option, ',');
	std::getline(line_stream, width, ',');
	std::getline(line_stream, height, ',');
	std::getline(line_stream, completion_time, ',');

	if(nullptr == FindTexture(clip_tag))
		throw std::exception{ "ResourceManager::_CreateAnimation2DClip" };

	auto animation_2d_clip = std::make_shared<ANIMATION_2D_CLIP_DESC>();
	animation_2d_clip->clip_tag = clip_tag;
	animation_2d_clip->texture_tag = texture_tag;
	animation_2d_clip->type = static_cast<ANIMATION_2D_TYPE>(std::stoi(type));
	animation_2d_clip->option = static_cast<ANIMATION_OPTION>(std::stoi(option));
	animation_2d_clip->width = std::stof(width);
	animation_2d_clip->height = std::stof(height);
	animation_2d_clip->completion_time = std::stof(completion_time);

	std::getline(file, line);
	std::getline(file, line);

	while (true)
	{
		line.clear();
		line_stream.clear();

		std::string left{};
		std::string top{};
		std::string right{};
		std::string bottom{};

		getline(file, line);
		line_stream.str(line);

		if (line.empty())
			break;

		getline(line_stream, left, ',');
		getline(line_stream, top, ',');
		getline(line_stream, right, ',');
		getline(line_stream, bottom, ',');

		ANIMATION_2D_FRAME_DESC frame{};
		frame.LT = Vector2{ std::stof(left), std::stof(top) };
		frame.RB = Vector2{ std::stof(right), std::stof(bottom) };

		animation_2d_clip->frame_vector.push_back(std::move(frame));
	}

	animation_2d_clip_map_.insert(std::make_pair(_tag, std::move(animation_2d_clip)));
}
