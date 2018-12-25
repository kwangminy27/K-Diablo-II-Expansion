#include "KEngine.h"
#include "ice_blast.h"

#include "Resource/resource_manager.h"
#include "Rendering/rendering_manager.h"
#include "Object/object_manager.h"
#include "Object/Component/transform.h"
#include "Object/Component/material.h"
#include "Object/Component/renderer.h"
#include "Object/Component/animation_2d.h"
#include "Object/Component/Collider/collider_aabb.h"

void K::IceBlast::Initialize()
{
	try
	{
		auto const& object_manager = ObjectManager::singleton();

		auto transform = object_manager->CreateComponent<Transform>(TAG{ TRANSFORM, 0 });
		AddComponent(transform);

		auto material = object_manager->CreateComponent<Material>(TAG{ MATERIAL, 0 });
		CPTR_CAST<Material>(material)->SetSampler(LINEAR_SAMPLER, 0, 0, 0);
		MaterialConstantBuffer Material_CB{};
		Material_CB.diffuse = DirectX::Colors::White.v;
		CPTR_CAST<Material>(material)->SetMaterialConstantBuffer(Material_CB, 0, 0);
		AddComponent(material);

		auto renderer = object_manager->CreateComponent<Renderer>(TAG{ RENDERER, 0 });
		CPTR_CAST<Renderer>(renderer)->set_mesh(ResourceManager::singleton()->FindMesh(TEX_RECT));
		CPTR_CAST<Renderer>(renderer)->set_shader(RenderingManager::singleton()->FindShader(BASIC_ANIMATION_2D_SHADER));
		CPTR_CAST<Renderer>(renderer)->set_render_state(RenderingManager::singleton()->FindRenderState(DEPTH_DISABLE));
		CPTR_CAST<Renderer>(renderer)->set_render_state(RenderingManager::singleton()->FindRenderState(RGB_ADD_BLEND));
		AddComponent(renderer);

		auto animation_2d = object_manager->CreateComponent<Animation2D>(TAG{ ANIMATION_2D, 0 });
		CPTR_CAST<Animation2D>(animation_2d)->set_owner(shared_from_this());
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("ice_blast");
		AddComponent(animation_2d);

		auto collider = object_manager->CreateComponent<ColliderAABB>(TAG{ COLLIDER, 0 });
		AddComponent(collider);

		set_speed(1200.f);
		set_range(1800.f);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "IceBlast::Initialize" << std::endl;
	}
}

K::APTR K::IceBlast::Clone() const
{
	return APTR{ new IceBlast{ *this }, [](IceBlast* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::IceBlast::Serialize(InputMemoryStream& _imstream)
{
}

void K::IceBlast::Serialize(OutputMemoryStream& _omstream)
{
}

void K::IceBlast::set_speed(float _speed)
{
	speed_ = _speed;
}

void K::IceBlast::set_range(float _range)
{
	range_ = _range;
}

void K::IceBlast::set_direction(Vector3 const& _direction)
{
	direction_ = _direction;
}

K::IceBlast::IceBlast(IceBlast const& _other) : MissileActor(_other)
{
	speed_ = _other.speed_;
	range_ = _other.range_;
	direction_ = _other.direction_;
}

K::IceBlast::IceBlast(IceBlast&& _other) noexcept : MissileActor(std::move(_other))
{
	speed_ = std::move(_other.speed_);
	range_ = std::move(_other.range_);
	direction_ = std::move(_other.direction_);
}

void K::IceBlast::_Finalize()
{
}

void K::IceBlast::_Update(float _time)
{
	auto const& transform = CPTR_CAST<Transform>(FindComponent(TAG{ TRANSFORM, 0 }));

	auto local_translation = transform->local_translation();

	auto angle = DirectX::XMConvertToDegrees(acosf(-Vector3::UnitY.Dot(direction_)));
	float isometric_correction_factor = sqrtf(1.f - cos(DirectX::XMConvertToRadians(angle)) * cos(DirectX::XMConvertToRadians(angle)) * 0.25f) * 0.5f;

	transform->set_local_translation(local_translation + Vector3{ direction_.x, direction_.y * isometric_correction_factor, direction_.z } *speed_ * _time);

	range_ -= speed_ * _time;

	if (range_ < 0.f)
		set_tag_state(TAG_STATE::DEAD);
}
