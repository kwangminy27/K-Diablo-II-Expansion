#include "KEngine.h"
#include "andariel_poisonball.h"

#include "Resource/resource_manager.h"
#include "Rendering/rendering_manager.h"
#include "World/world_manager.h"
#include "World/layer.h"
#include "Object/object_manager.h"
#include "Object/Actor/Player/player_actor.h"
#include "Object/Actor/Monster/monster_actor.h"
#include "fallen_shaman_fireball_explosion.h"
#include "Object/Component/transform.h"
#include "Object/Component/material.h"
#include "Object/Component/renderer.h"
#include "Object/Component/animation_2d.h"
#include "Object/Component/Collider/collider_aabb.h"

void K::AndarielPoisonball::Initialize()
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
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_poisonball");
		AddComponent(animation_2d);

		auto collider = object_manager->CreateComponent<ColliderAABB>(TAG{ COLLIDER, 0 });
		CPTR_CAST<ColliderAABB>(collider)->set_owner_type(OWNER_TYPE::MISSILE);
		CPTR_CAST<ColliderAABB>(collider)->set_relative_info(AABB{ Vector3::Zero, Vector3{ 12.f, 15.f, 0.f } });
		CPTR_CAST<ColliderAABB>(collider)->AddCallback([](Collider* _src, Collider* _dest, float _time) {
			if (OWNER_TYPE::PLAYER != _dest->owner_type())
				return;

			auto player = APTR_CAST<PlayerActor>(_dest->owner());

			switch (player->state())
			{
			case ACTOR_STATE::DEAD:
			case ACTOR_STATE::DEATH:
				return;
			}

			player->AddHp(-25.f);

			if (player->hp() <= 0.f)
				player->set_state(ACTOR_STATE::DEATH);
			else
				player->set_state(ACTOR_STATE::GET_HIT);

			player->set_element_state(ELEMENT_STATE::POISON);

			_src->owner()->set_tag_state(TAG_STATE::DEAD);
		}, COLLISION_CALLBACK_TYPE::ENTER);
		AddComponent(collider);

		set_speed(250.f);
		set_range(500.f);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "AndarielPoisonball::Initialize" << std::endl;
	}
}

K::APTR K::AndarielPoisonball::Clone() const
{
	return APTR{ new AndarielPoisonball{ *this }, [](AndarielPoisonball* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::AndarielPoisonball::Serialize(InputMemoryStream& _imstream)
{
}

void K::AndarielPoisonball::Serialize(OutputMemoryStream& _omstream)
{
}

void K::AndarielPoisonball::set_speed(float _speed)
{
	speed_ = _speed;
}

void K::AndarielPoisonball::set_range(float _range)
{
	range_ = _range;
}

void K::AndarielPoisonball::set_direction(Vector3 const& _direction)
{
	direction_ = _direction;
}

K::AndarielPoisonball::AndarielPoisonball(AndarielPoisonball const& _other) : MissileActor(_other)
{
	speed_ = _other.speed_;
	range_ = _other.range_;
	direction_ = _other.direction_;
}

K::AndarielPoisonball::AndarielPoisonball(AndarielPoisonball&& _other) noexcept : MissileActor(std::move(_other))
{
	speed_ = std::move(_other.speed_);
	range_ = std::move(_other.range_);
	direction_ = std::move(_other.direction_);
}

void K::AndarielPoisonball::_Finalize()
{
}

void K::AndarielPoisonball::_Update(float _time)
{
	auto const& transform = CPTR_CAST<Transform>(FindComponent(TAG{ TRANSFORM, 0 }));

	auto local_translation = transform->local_translation();

	auto angle = DirectX::XMConvertToDegrees(acosf(-Vector3::UnitY.Dot(direction_)));

	transform->set_local_translation(local_translation + direction_ * speed_ * _time);

	range_ -= speed_ * _time;

	if (range_ < 0.f)
		set_tag_state(TAG_STATE::DEAD);
}
