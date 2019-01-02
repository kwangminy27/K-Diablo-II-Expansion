#include "KEngine.h"
#include "fallen_shaman.h"
#include "fallen_shaman_shadow.h"

#include "Audio/audio_manager.h"
#include "Resource/resource_manager.h"
#include "Rendering/rendering_manager.h"
#include "World/world_manager.h"
#include "World/layer.h"
#include "Object/object_manager.h"
#include "Object/Actor/Missile/fallen_shaman_missile.h"
#include "Object/Actor/Missile/fallen_shaman_fireball.h"
#include "Object/Component/transform.h"
#include "Object/Component/material.h"
#include "Object/Component/renderer.h"
#include "Object/Component/animation_2d.h"
#include "Object/Component/Collider/collider_aabb.h"
#include "Object/Component/Collider/collider_circle.h"
#include "Object/Component/navigator.h"

void K::FallenShaman::Initialize()
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
		CPTR_CAST<Renderer>(renderer)->set_render_state(RenderingManager::singleton()->FindRenderState(ALPHA_BLEND));
		AddComponent(renderer);

		auto animation_2d = object_manager->CreateComponent<Animation2D>(TAG{ ANIMATION_2D, 0 });
		CPTR_CAST<Animation2D>(animation_2d)->set_owner(shared_from_this());
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("fallen_shaman_attack1");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("fallen_shaman_attack2");
		//CPTR_CAST<Animation2D>(animation_2d)->AddClip("fallen_shaman_attack2(overlay)");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("fallen_shaman_dead");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("fallen_shaman_death");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("fallen_shaman_get_hit");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("fallen_shaman_neutral");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("fallen_shaman_walk");
		CPTR_CAST<Animation2D>(animation_2d)->SetCurrentClip("fallen_shaman_neutral", 0);
		CPTR_CAST<Animation2D>(animation_2d)->SetDefaultClip("fallen_shaman_neutral", 0);
		AddComponent(animation_2d);

		auto collider = object_manager->CreateComponent<ColliderAABB>(TAG{ COLLIDER, 0 });
		CPTR_CAST<ColliderAABB>(collider)->set_relative_info(AABB{ Vector3::Zero, Vector3{ 10.f, 25.f, 0.f } });
		CPTR_CAST<ColliderAABB>(collider)->set_owner_type(OWNER_TYPE::MONSTER);
		AddComponent(collider);

		auto view_range = object_manager->CreateComponent<ColliderCircle>(TAG{ COLLIDER, 1 });
		CPTR_CAST<ColliderAABB>(view_range)->set_owner_type(OWNER_TYPE::VIEW);
		CPTR_CAST<ColliderCircle>(view_range)->set_relative_info(Circle{ Vector3::Zero, 600.f });
		CPTR_CAST<ColliderCircle>(view_range)->AddCallback([this](Collider* _src, Collider* _dest, float _time) {
			if (target_.expired())
			{
				if (OWNER_TYPE::PLAYER == _dest->owner_type())
					set_target(_dest->owner());
			}
		}, COLLISION_CALLBACK_TYPE::ENTER);
		//CPTR_CAST<ColliderCircle>(view_range)->AddCallback([this](Collider* _src, Collider* _dest, float _time) {
		//	if (OWNER_TYPE::PLAYER == _dest->owner_type())
		//		set_target(nullptr);
		//}, COLLISION_CALLBACK_TYPE::LEAVE);
		AddComponent(view_range);

		auto navigator = object_manager->CreateComponent<Navigator>(TAG{ NAVIGATOR, 0 });
		CPTR_CAST<Navigator>(navigator)->set_speed(100.f);
		AddComponent(navigator);

		auto shadow = object_manager->CreateActor<FallenShamanShadow>(TAG{ "FallenShamanShadow", 0 });
		auto const& shadow_transform = CPTR_CAST<Transform>(shadow->FindComponent(TAG{ TRANSFORM, 0 }));
		shadow_transform->set_parent_flag(static_cast<uint8_t>(PARENT_FLAG::TRANSLATION));
		shadow_transform->set_local_translation(Vector3{ 25.f, 0.f, 0.f });
		shadow_transform->set_local_rotation(Quaternion::CreateFromYawPitchRoll(0.f, 0.f, -30.f));
		AddChild(shadow);

		set_state(ACTOR_STATE::NEUTRAL);

		set_hp(100.f);
		set_speed(100.f);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "FallenShaman::Initialize" << std::endl;
	}
}

K::APTR K::FallenShaman::Clone() const
{
	return APTR{ new FallenShaman{ *this }, [](FallenShaman* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::FallenShaman::Serialize(InputMemoryStream& _imstream)
{
	for (auto& component : component_list_)
		component->Serialize(_imstream);

	//for (auto& child : child_list_)
	//	child->Serialize(_imstream);
}

void K::FallenShaman::Serialize(OutputMemoryStream& _omstream)
{
	for (auto& component : component_list_)
		component->Serialize(_omstream);

	//for (auto& child : child_list_)
	//	child->Serialize(_omstream);
}

K::FallenShaman::FallenShaman(FallenShaman const& _other) : MonsterActor(_other)
{
}

K::FallenShaman::FallenShaman(FallenShaman&& _other) noexcept : MonsterActor(std::move(_other))
{
}

void K::FallenShaman::_Finalize()
{
}

void K::FallenShaman::_Input(float _time)
{
	auto const& transform = CPTR_CAST<Transform>(FindComponent(TAG{ TRANSFORM, 0 }));
	auto const& animation_2d = CPTR_CAST<Animation2D>(FindComponent(TAG{ ANIMATION_2D, 0 }));
	auto const& navigator = CPTR_CAST<Navigator>(FindComponent(TAG{ NAVIGATOR, 0 }));

	switch (state_)
	{
	case K::ACTOR_STATE::ATTACK1:
		break;
	case K::ACTOR_STATE::ATTACK2:
		break;
	case K::ACTOR_STATE::GET_HIT:
		break;
	case K::ACTOR_STATE::DEAD:
		break;
	case K::ACTOR_STATE::DEATH:
		break;
	case K::ACTOR_STATE::RUN:
	case K::ACTOR_STATE::SPECIAL_CAST:
	case K::ACTOR_STATE::SPECIAL1:
		break;
	case K::ACTOR_STATE::NEUTRAL:
	case K::ACTOR_STATE::WALK:
		if (target_.expired())
		{
			navigator->ClearPath();

			set_state(ACTOR_STATE::NEUTRAL);
		}
		else
		{
			auto position = transform->world().Translation();
			auto target_position = CPTR_CAST<Transform>(target()->FindComponent(TAG{ TRANSFORM, 0 }))->world().Translation();

			navigator->Route(position, target_position);

			auto distance = Vector3::Distance(position, target_position);

			if (distance <= 16.f)
			{
				set_state(ACTOR_STATE::ATTACK1);

				navigator->ClearPath();
			}
			else if (distance <= 400.f)
			{
				set_state(ACTOR_STATE::ATTACK2);

				navigator->ClearPath();
			}
			else if (distance >= 650.f)
				target_.reset();
			else
				set_state(ACTOR_STATE::WALK);
		}
		break;
	}
}

void K::FallenShaman::_Update(float _time)
{
	auto const& transform = CPTR_CAST<Transform>(FindComponent(TAG{ TRANSFORM, 0 }));
	auto const& animation_2d = CPTR_CAST<Animation2D>(FindComponent(TAG{ ANIMATION_2D, 0 }));
	auto const& navigator = CPTR_CAST<Navigator>(FindComponent(TAG{ NAVIGATOR, 0 }));

	Vector3 direction{};

	if (target_.expired())
		direction = navigator->direction();
	else
	{
		if (ACTOR_STATE::DEATH != state() && ACTOR_STATE::DEAD != state())
		{
			auto caching_target = target();

			auto target_position = CPTR_CAST<Transform>(caching_target->FindComponent(TAG{ TRANSFORM, 0 }))->world().Translation();
			auto position = transform->world().Translation();

			direction = target_position - position;
			direction.Normalize();
		}
	}

	auto angle = DirectX::XMConvertToDegrees(acosf(-Vector3::UnitY.Dot(direction)));

	int dir_idx{};
	int _16_dir_idx{};

	if (direction.x < 0.f)
	{
		if (angle < 22.5f)
			dir_idx = 0;
		else if (angle < 67.5f)
			dir_idx = 1;
		else if (angle < 112.5f)
			dir_idx = 2;
		else if (angle < 157.5f)
			dir_idx = 3;
		else
			dir_idx = 4;
	}
	else
	{
		if (angle < 22.5f)
			dir_idx = 0;
		else if (angle < 67.5f)
			dir_idx = 7;
		else if (angle < 112.5f)
			dir_idx = 6;
		else if (angle < 157.5f)
			dir_idx = 5;
		else
			dir_idx = 4;
	}

	if (direction.x < 0.f)
	{
		if (angle < 11.25f)
			_16_dir_idx = 0;
		else if (angle < 33.75f)
			_16_dir_idx = 1;
		else if (angle < 56.25f)
			_16_dir_idx = 2;
		else if (angle < 78.25f)
			_16_dir_idx = 3;
		else if (angle < 100.75f)
			_16_dir_idx = 4;
		else if (angle < 123.25f)
			_16_dir_idx = 5;
		else if (angle < 145.75f)
			_16_dir_idx = 6;
		else if (angle < 168.25f)
			_16_dir_idx = 7;
		else
			_16_dir_idx = 8;
	}
	else
	{
		if (angle < 11.25f)
			_16_dir_idx = 0;
		else if (angle < 33.75f)
			_16_dir_idx = 15;
		else if (angle < 56.25f)
			_16_dir_idx = 14;
		else if (angle < 78.25f)
			_16_dir_idx = 13;
		else if (angle < 100.75f)
			_16_dir_idx = 12;
		else if (angle < 123.25f)
			_16_dir_idx = 11;
		else if (angle < 145.75f)
			_16_dir_idx = 10;
		else if (angle < 168.25f)
			_16_dir_idx = 9;
		else
			_16_dir_idx = 8;
	}

	shadow_dir_idx_ = dir_idx;

	switch (state_)
	{
	case K::ACTOR_STATE::ATTACK1:
		animation_2d->SetCurrentClip("fallen_shaman_attack1", dir_idx);
		break;
	case K::ACTOR_STATE::ATTACK2:
		animation_2d->SetCurrentClip("fallen_shaman_attack2", dir_idx);

		if (false == once_flag_array_.at(static_cast<int>(ACTOR_STATE::ATTACK2)) && 0 == animation_2d->frame_idx() % 17)
		{
			AudioManager::singleton()->FindSoundEffect("fallen_shaman_fireball")->Play();

			once_flag_array_.at(static_cast<int>(ACTOR_STATE::ATTACK2)) = true;
		}
		else if (0 != animation_2d->frame_idx() % 17)
			once_flag_array_.at(static_cast<int>(ACTOR_STATE::ATTACK2)) = false;

		break;
	case K::ACTOR_STATE::GET_HIT:
		animation_2d->SetCurrentClip("fallen_shaman_get_hit", dir_idx);

		if (false == once_flag_array_.at(static_cast<int>(ACTOR_STATE::GET_HIT)) && 0 == animation_2d->frame_idx() % 5)
		{
			std::random_device r{};
			std::default_random_engine gen{ r() };
			std::uniform_int_distribution uniform_dist{ 1, 4 };
			auto number = uniform_dist(gen);

			AudioManager::singleton()->FindSoundEffect("fallen_shaman_gethit" + std::to_string(number))->Play();

			once_flag_array_.at(static_cast<int>(ACTOR_STATE::GET_HIT)) = true;
		}
		else if (0 != animation_2d->frame_idx() % 5)
			once_flag_array_.at(static_cast<int>(ACTOR_STATE::GET_HIT)) = false;

		break;
	case K::ACTOR_STATE::DEAD:
		animation_2d->SetCurrentClip("fallen_shaman_dead", dir_idx);
		break;
	case K::ACTOR_STATE::DEATH:
		animation_2d->SetCurrentClip("fallen_shaman_death", dir_idx);

		if (false == once_flag_array_.at(static_cast<int>(ACTOR_STATE::DEATH)) && 0 == animation_2d->frame_idx() % 21)
		{
			std::random_device r{};
			std::default_random_engine gen{ r() };
			std::uniform_int_distribution uniform_dist{ 1, 4 };
			auto number = uniform_dist(gen);

			AudioManager::singleton()->FindSoundEffect("fallen_shaman_death" + std::to_string(number))->Play();

			once_flag_array_.at(static_cast<int>(ACTOR_STATE::DEATH)) = true;
		}
		else if (0 != animation_2d->frame_idx() % 21)
			once_flag_array_.at(static_cast<int>(ACTOR_STATE::DEATH)) = false;

		break;
	case K::ACTOR_STATE::NEUTRAL:
		animation_2d->SetCurrentClip("fallen_shaman_neutral", dir_idx);
		break;
	case K::ACTOR_STATE::RUN:
	case K::ACTOR_STATE::SPECIAL_CAST:
	case K::ACTOR_STATE::SPECIAL1:
		break;
	case K::ACTOR_STATE::WALK:
		animation_2d->SetCurrentClip("fallen_shaman_walk", dir_idx);
		break;
	}

	auto position = transform->world().Translation();

	animation_2d->set_callback([actor_state = state_, dir_idx, _16_dir_idx, position, direction]() {
		auto const& object_manager = ObjectManager::singleton();
		auto const& layer = WorldManager::singleton()->FindLayer(TAG{ "DefaultLayer", 1 });

		switch (actor_state)
		{
		case ACTOR_STATE::ATTACK2:
			auto missile_cast = object_manager->CreateActor<FallenShamanMissile>(TAG{ "FallenShamanMissile", 0 });
			auto const& missile_cast_transform = CPTR_CAST<Transform>(missile_cast->FindComponent(TAG{ TRANSFORM, 0 }));
			missile_cast_transform->set_local_translation(position);
			auto const& missile_cast_animation_2d = CPTR_CAST<Animation2D>(missile_cast->FindComponent(TAG{ ANIMATION_2D, 0 }));
			missile_cast_animation_2d->SetCurrentClip("fallen_shaman_attack2(overlay)", dir_idx);
			layer->AddActor(missile_cast);

			auto fireball = object_manager->CreateActor<FallenShamanFireball>(TAG{ "FallenShamanFireball", 0 });
			APTR_CAST<FallenShamanFireball>(fireball)->set_direction(direction);
			auto const& fireball_transform = CPTR_CAST<Transform>(fireball->FindComponent(TAG{ TRANSFORM, 0 }));
			fireball_transform->set_local_translation(position);
			auto const& fireball_animation_2d = CPTR_CAST<Animation2D>(fireball->FindComponent(TAG{ ANIMATION_2D, 0 }));
			fireball_animation_2d->SetCurrentClip("fallen_shaman_fireball", _16_dir_idx);
			layer->AddActor(fireball);
			break;
		}
	});
}

void K::FallenShaman::_Render(float _time)
{
	auto const& material = CPTR_CAST<Material>(FindComponent(TAG{ MATERIAL, 0 }));

	MaterialConstantBuffer material_CB{};

	if (element_time_ <= 0.f)
		set_element_state(ELEMENT_STATE::NORMAL);
	else
		element_time_ -= _time;

	switch (element_state_)
	{
	case ELEMENT_STATE::NORMAL:
		material_CB.diffuse = DirectX::Colors::White.v;
		break;
	case ELEMENT_STATE::COLD:
		material_CB.diffuse = DirectX::Colors::Blue.v;
		break;
	case ELEMENT_STATE::FIRE:
		material_CB.diffuse = DirectX::Colors::Red.v;
		break;
	case ELEMENT_STATE::POISON:
		material_CB.diffuse = DirectX::Colors::Lime.v;
		break;
	}

	material->SetMaterialConstantBuffer(material_CB, 0, 0);
}
