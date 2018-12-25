#include "KEngine.h"
#include "ice_orb.h"

#include "Resource/resource_manager.h"
#include "Rendering/rendering_manager.h"
#include "World/world_manager.h"
#include "World/layer.h"
#include "Object/object_manager.h"
#include "Object/Actor/Missile/ice_bolt.h"
#include "Object/Component/transform.h"
#include "Object/Component/material.h"
#include "Object/Component/renderer.h"
#include "Object/Component/animation_2d.h"
#include "Object/Component/Collider/collider_aabb.h"

void K::IceOrb::Initialize()
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
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("ice_orb");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("ice_orb_explosion");
		AddComponent(animation_2d);

		auto collider = object_manager->CreateComponent<ColliderAABB>(TAG{ COLLIDER, 0 });
		AddComponent(collider);

		set_speed(300.f);
		set_range(600.f);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "IceOrb::Initialize" << std::endl;
	}
}

K::APTR K::IceOrb::Clone() const
{
	return APTR{ new IceOrb{ *this }, [](IceOrb* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::IceOrb::Serialize(InputMemoryStream& _imstream)
{
}

void K::IceOrb::Serialize(OutputMemoryStream& _omstream)
{
}

void K::IceOrb::set_speed(float _speed)
{
	speed_ = _speed;
}

void K::IceOrb::set_range(float _range)
{
	range_ = _range;
}

void K::IceOrb::set_direction(Vector3 const& _direction)
{
	direction_ = _direction;
}

K::IceOrb::IceOrb(IceOrb const& _other) : MissileActor(_other)
{
	speed_ = _other.speed_;
	range_ = _other.range_;
	direction_ = _other.direction_;
}

K::IceOrb::IceOrb(IceOrb&& _other) noexcept : MissileActor(std::move(_other))
{
	speed_ = std::move(_other.speed_);
	range_ = std::move(_other.range_);
	direction_ = std::move(_other.direction_);
}

void K::IceOrb::_Finalize()
{
}

void K::IceOrb::_Update(float _time)
{
	auto const& transform = CPTR_CAST<Transform>(FindComponent(TAG{ TRANSFORM, 0 }));

	auto local_translation = transform->local_translation();

	auto angle = DirectX::XMConvertToDegrees(acosf(-Vector3::UnitY.Dot(direction_)));
	float isometric_correction_factor = sqrtf(1.f - cos(DirectX::XMConvertToRadians(angle)) * cos(DirectX::XMConvertToRadians(angle)) * 0.25f) * 0.5f;

	transform->set_local_translation(local_translation + Vector3{ direction_.x, direction_.y * isometric_correction_factor, direction_.z } *speed_ * _time);

	range_ -= speed_ * _time;

	auto const& object_manager = ObjectManager::singleton();

	auto position = transform->world().Translation();

	elapsed_time_ += _time;

	if (elapsed_time_ >= interval_time_)
	{
		for (auto i = 0; i < 4; ++i)
		{
			angle_ += 130.f;

			angle_ = static_cast<float>(static_cast<int>(angle_) % 360);

			auto direction = Vector3::TransformNormal(-Vector3::UnitY, Matrix::CreateRotationZ(-angle_));

			auto ice_bolt = object_manager->CreateActor<IceBolt>(TAG{ "IceBolt", 0 });
			auto const& ice_bolt_transform = CPTR_CAST<Transform>(ice_bolt->FindComponent(TAG{ TRANSFORM, 0 }));
			auto const& ice_bolt_animation_2d = CPTR_CAST<Animation2D>(ice_bolt->FindComponent(TAG{ ANIMATION_2D, 0 }));

			APTR_CAST<IceBolt>(ice_bolt)->set_direction(direction);
			ice_bolt_transform->set_local_translation(position);

			int dir_idx = static_cast<int>(angle_ / 22.5f);

			ice_bolt_animation_2d->SetCurrentClip("ice_bolt", dir_idx);

			auto const& default_layer = WorldManager::singleton()->FindLayer(TAG{ "DefaultLayer", 1 });
			default_layer->AddActor(ice_bolt);
		}

		elapsed_time_ -= interval_time_;
	}

	if (range_ < 0.f && once_flag_)
	{
		for (auto i = 0; i < 16; ++i)
		{
			angle_ += 130.f;

			angle_ = static_cast<float>(static_cast<int>(angle_) % 360);

			auto direction = Vector3::TransformNormal(-Vector3::UnitY, Matrix::CreateRotationZ(-angle_));

			auto ice_bolt = object_manager->CreateActor<IceBolt>(TAG{ "IceBolt", 0 });
			auto const& ice_bolt_transform = CPTR_CAST<Transform>(ice_bolt->FindComponent(TAG{ TRANSFORM, 0 }));
			auto const& ice_bolt_animation_2d = CPTR_CAST<Animation2D>(ice_bolt->FindComponent(TAG{ ANIMATION_2D, 0 }));

			APTR_CAST<IceBolt>(ice_bolt)->set_direction(direction);
			APTR_CAST<IceBolt>(ice_bolt)->set_spin_flag(true);
			ice_bolt_transform->set_local_translation(position);

			int dir_idx = static_cast<int>(angle_ / 22.5f);

			ice_bolt_animation_2d->SetCurrentClip("ice_bolt", dir_idx);

			auto const& default_layer = WorldManager::singleton()->FindLayer(TAG{ "DefaultLayer", 1 });
			default_layer->AddActor(ice_bolt);
		}

		auto const& animation_2d = CPTR_CAST<Animation2D>(FindComponent(TAG{ ANIMATION_2D, 0 }));
		animation_2d->SetCurrentClip("ice_orb_explosion", -1);

		once_flag_ = false;
	}
}
