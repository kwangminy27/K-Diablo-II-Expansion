#include "KEngine.h"
#include "ice_bolt.h"

#include "Resource/resource_manager.h"
#include "Rendering/rendering_manager.h"
#include "Object/object_manager.h"
#include "Object/Actor/Monster/monster_actor.h"
#include "Object/Component/transform.h"
#include "Object/Component/material.h"
#include "Object/Component/renderer.h"
#include "Object/Component/animation_2d.h"
#include "Object/Component/Collider/collider_aabb.h"

void K::IceBolt::Initialize()
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
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("ice_bolt");
		AddComponent(animation_2d);

		auto collider = object_manager->CreateComponent<ColliderAABB>(TAG{ COLLIDER, 0 });
		CPTR_CAST<ColliderAABB>(collider)->set_owner_type(OWNER_TYPE::MISSILE);
		CPTR_CAST<ColliderAABB>(collider)->set_relative_info(AABB{ Vector3::Zero, Vector3{ 12.f, 8.f, 0.f }});
		CPTR_CAST<ColliderAABB>(collider)->AddCallback([](Collider* _src, Collider* _dest, float _time) {
			if (OWNER_TYPE::MONSTER != _dest->owner_type())
				return;

			auto monster = APTR_CAST<MonsterActor>(_dest->owner());

			switch (monster->state())
			{
			case ACTOR_STATE::DEAD:
			case ACTOR_STATE::DEATH:
				return;
			}

			monster->AddHp(-25.f);

			if (monster->hp() <= 0.f)
				monster->set_state(ACTOR_STATE::DEATH);
			else
				monster->set_state(ACTOR_STATE::GET_HIT);

			_src->owner()->set_tag_state(TAG_STATE::DEAD);
		}, COLLISION_CALLBACK_TYPE::ENTER);
		AddComponent(collider);

		set_speed(600.f);
		set_range(1500.f);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "IceBolt::Initialize" << std::endl;
	}
}

K::APTR K::IceBolt::Clone() const
{
	return APTR{ new IceBolt{ *this }, [](IceBolt* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::IceBolt::Serialize(InputMemoryStream& _imstream)
{
}

void K::IceBolt::Serialize(OutputMemoryStream& _omstream)
{
}

void K::IceBolt::set_speed(float _speed)
{
	speed_ = _speed;
}

void K::IceBolt::set_range(float _range)
{
	range_ = _range;
}

void K::IceBolt::set_spin_flag(bool _flag)
{
	spin_flag_ = _flag;
}

void K::IceBolt::set_direction(Vector3 const& _direction)
{
	direction_ = _direction;
}

K::IceBolt::IceBolt(IceBolt const& _other) : MissileActor(_other)
{
	speed_ = _other.speed_;
	range_ = _other.range_;
	spin_flag_ = _other.spin_flag_;
	direction_ = _other.direction_;
}

K::IceBolt::IceBolt(IceBolt&& _other) noexcept : MissileActor(std::move(_other))
{
	speed_ = std::move(_other.speed_);
	range_ = std::move(_other.range_);
	spin_flag_ = std::move(_other.spin_flag_);
	direction_ = std::move(_other.direction_);
}

void K::IceBolt::_Finalize()
{
}

void K::IceBolt::_Update(float _time)
{
	auto const& transform = CPTR_CAST<Transform>(FindComponent(TAG{ TRANSFORM, 0 }));

	auto local_translation = transform->local_translation();

	range_ -= speed_ * _time;

	auto angle = DirectX::XMConvertToDegrees(acosf(-Vector3::UnitY.Dot(direction_)));
	float isometric_correction_factor = sqrtf(1.f - cos(DirectX::XMConvertToRadians(angle)) * cos(DirectX::XMConvertToRadians(angle)) * 0.25f) * 0.5f;

	if (spin_flag_)
	{
		if (range_ <= 1455.f && spin_point_1_)
		{
			direction_ = Vector3::TransformNormal(direction_, Matrix::CreateRotationZ(-90.f));

			angle = DirectX::XMConvertToDegrees(acosf(-Vector3::UnitY.Dot(direction_)));

			int dir_idx{};

			if (direction_.x < 0.f)
			{
				if (angle < 11.25f)
					dir_idx = 0;
				else if (angle < 33.75f)
					dir_idx = 1;
				else if (angle < 56.25f)
					dir_idx = 2;
				else if (angle < 78.25f)
					dir_idx = 3;
				else if (angle < 100.75f)
					dir_idx = 4;
				else if (angle < 123.25f)
					dir_idx = 5;
				else if (angle < 145.75f)
					dir_idx = 6;
				else if (angle < 168.25f)
					dir_idx = 7;
				else
					dir_idx = 8;
			}
			else
			{
				if (angle < 11.25f)
					dir_idx = 0;
				else if (angle < 33.75f)
					dir_idx = 15;
				else if (angle < 56.25f)
					dir_idx = 14;
				else if (angle < 78.25f)
					dir_idx = 13;
				else if (angle < 100.75f)
					dir_idx = 12;
				else if (angle < 123.25f)
					dir_idx = 11;
				else if (angle < 145.75f)
					dir_idx = 10;
				else if (angle < 168.25f)
					dir_idx = 9;
				else
					dir_idx = 8;
			}

			auto const& animation_2d = CPTR_CAST<Animation2D>(FindComponent(TAG{ ANIMATION_2D, 0 }));
			animation_2d->SetCurrentClip("ice_bolt", dir_idx);

			spin_point_1_ = false;
		}
		else if(range_ <= 1410.f && spin_point_2_)
		{
			direction_ = Vector3::TransformNormal(direction_, Matrix::CreateRotationZ(-45.f));

			angle = DirectX::XMConvertToDegrees(acosf(-Vector3::UnitY.Dot(direction_)));

			int dir_idx{};

			if (direction_.x < 0.f)
			{
				if (angle < 11.25f)
					dir_idx = 0;
				else if (angle < 33.75f)
					dir_idx = 1;
				else if (angle < 56.25f)
					dir_idx = 2;
				else if (angle < 78.25f)
					dir_idx = 3;
				else if (angle < 100.75f)
					dir_idx = 4;
				else if (angle < 123.25)
					dir_idx = 5;
				else if (angle < 145.75)
					dir_idx = 6;
				else if (angle < 168.25)
					dir_idx = 7;
				else
					dir_idx = 8;
			}
			else
			{
				if (angle < 11.25f)
					dir_idx = 0;
				else if (angle < 33.75f)
					dir_idx = 15;
				else if (angle < 56.25f)
					dir_idx = 14;
				else if (angle < 78.25f)
					dir_idx = 13;
				else if (angle < 100.75f)
					dir_idx = 12;
				else if (angle < 123.25)
					dir_idx = 11;
				else if (angle < 145.75)
					dir_idx = 10;
				else if (angle < 168.25)
					dir_idx = 9;
				else
					dir_idx = 8;
			}

			auto const& animation_2d = CPTR_CAST<Animation2D>(FindComponent(TAG{ ANIMATION_2D, 0 }));
			animation_2d->SetCurrentClip("ice_bolt", dir_idx);

			spin_point_2_ = false;
		}
		else
			transform->set_local_translation(local_translation + Vector3{ direction_.x, direction_.y * isometric_correction_factor, direction_.z } * speed_ * _time);
	}
	else
		transform->set_local_translation(local_translation + direction_ * speed_ * _time);

	if (range_ < 0.f)
		set_tag_state(TAG_STATE::DEAD);
}
