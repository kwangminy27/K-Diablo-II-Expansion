#include "KEngine.h"
#include "tile_actor.h"

#include "Object/object_manager.h"
#include "Object/Component/transform.h"
#include "Object/Component/material.h"

void K::TileActor::Initialize()
{
	try
	{
		auto transform = ObjectManager::singleton()->CreateComponent<Transform>(TAG{ TRANSFORM, 0 });
		AddComponent(transform);

		auto material = ObjectManager::singleton()->CreateComponent<Material>(TAG{ MATERIAL, 0 });
		CPTR_CAST<Material>(material)->SetSampler(LINEAR_SAMPLER, 0, 0, 0);
		MaterialConstantBuffer Material_CB{};
		Material_CB.diffuse = DirectX::Colors::White.v;
		CPTR_CAST<Material>(material)->SetMaterialConstantBuffer(Material_CB, 0, 0);
		AddComponent(transform);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "TileActor::Initialize" << std::endl;
	}
}

K::APTR K::TileActor::Clone() const
{
	return APTR{ new TileActor{ *this }, [](TileActor* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::TileActor::Serialize(InputMemoryStream& _imstream)
{
	_imstream.Serialize(option_);

	_imstream.Serialize(texture_tag_);
	set_texture_tag(texture_tag_);

	CPTR_CAST<Transform>(FindComponent(TAG{ TRANSFORM, 0 }))->Serialize(_imstream);
}

void K::TileActor::Serialize(OutputMemoryStream& _omstream)
{
	_omstream.Serialize(option_);

	_omstream.Serialize(texture_tag_);

	CPTR_CAST<Transform>(FindComponent(TAG{ TRANSFORM, 0 }))->Serialize(_omstream);
}

K::TILE_OPTION K::TileActor::option() const
{
	return option_;
}

void K::TileActor::set_texture_tag(std::string const& _tag)
{
	texture_tag_ = _tag;

	CPTR_CAST<Material>(FindComponent(TAG{ MATERIAL, 0 }))->SetTexture(_tag, 0, 0, 0);
}

void K::TileActor::set_option(TILE_OPTION _option)
{
	option_ = _option;
}

K::TileActor::TileActor(TileActor const& _other) : Actor(_other)
{
	option_ = _other.option_;
	texture_tag_ = _other.texture_tag_;
}

K::TileActor::TileActor(TileActor&& _other) noexcept : Actor(std::move(_other))
{
	option_ = std::move(_other.option_);
	texture_tag_ = std::move(_other.texture_tag_);
}

void K::TileActor::_Finalize()
{
}
