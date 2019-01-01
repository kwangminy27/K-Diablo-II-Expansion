#include "KEngine.h"
#include "andariel.h"

#include "Audio/audio_manager.h"
#include "Resource/resource_manager.h"
#include "Rendering/rendering_manager.h"
#include "World/world_manager.h"
#include "World/layer.h"
#include "Object/object_manager.h"
#include "Object/Actor/Monster/cow.h"
#include "Object/Actor/Monster/wendigo.h"
#include "Object/Actor/Monster/fallen_shaman.h"
#include "Object/Actor/Missile/andariel_death_overlay.h"
#include "Object/Actor/Missile/andariel_special_cast_overlay.h"
#include "Object/Actor/Missile/andariel_poisonball.h"
#include "Object/Component/transform.h"
#include "Object/Component/material.h"
#include "Object/Component/renderer.h"
#include "Object/Component/animation_2d.h"
#include "Object/Component/Collider/collider_aabb.h"
#include "Object/Component/Collider/collider_circle.h"
#include "Object/Component/navigator.h"

void K::Andariel::Initialize()
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
		//CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_attack1");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_death");
		//CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_death(overlay)");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_get_hit");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_neutral");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_walk");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_special_cast");
		//CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_special_cast(overlay)");
		CPTR_CAST<Animation2D>(animation_2d)->SetCurrentClip("andariel_walk", 0);
		CPTR_CAST<Animation2D>(animation_2d)->SetDefaultClip("andariel_walk", 0);
		AddComponent(animation_2d);

		auto collider = object_manager->CreateComponent<ColliderAABB>(TAG{ COLLIDER, 0 });
		CPTR_CAST<ColliderAABB>(collider)->set_relative_info(AABB{ Vector3::Zero, Vector3{ 30.f, 75.f, 0.f } });
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
		CPTR_CAST<ColliderCircle>(view_range)->AddCallback([this](Collider* _src, Collider* _dest, float _time) {
			if (OWNER_TYPE::PLAYER == _dest->owner_type())
				set_target(nullptr);
		}, COLLISION_CALLBACK_TYPE::LEAVE);
		AddComponent(view_range);

		auto navigator = object_manager->CreateComponent<Navigator>(TAG{ NAVIGATOR, 0 });
		CPTR_CAST<Navigator>(navigator)->set_speed(300.f);
		AddComponent(navigator);

		set_state(ACTOR_STATE::NEUTRAL);

		set_hp(1000.f);
		set_speed(300.f);

		//// ╨нго
		//for (auto i = 0; i < 10; ++i)
		//{
		//	for (auto j = 0; j < 10; ++j)
		//	{
		//		auto cow = object_manager->CreateActor<Cow>(TAG{ "Cow", object_manager->counter() });
		//		auto const& cow_transform = CPTR_CAST<Transform>(cow->FindComponent(TAG{ TRANSFORM, 0 }));
		//		cow_transform->set_parent_flag(static_cast<uint8_t>(PARENT_FLAG::TRANSLATION));
		//		cow_transform->set_local_translation(Vector3{ -1000.f + 50.f * j, -250.f + 50.f * i, 0.f });
		//		AddChild(cow);
		//	}
		//}

		//for (auto i = 0; i < 10; ++i)
		//{
		//	for (auto j = 0; j < 10; ++j)
		//	{
		//		auto wendigo = object_manager->CreateActor<Wendigo>(TAG{ "Wendigo", object_manager->counter() });
		//		auto const& wendigo_transform = CPTR_CAST<Transform>(wendigo->FindComponent(TAG{ TRANSFORM, 0 }));
		//		wendigo_transform->set_parent_flag(static_cast<uint8_t>(PARENT_FLAG::TRANSLATION));
		//		wendigo_transform->set_local_translation(Vector3{ 500.f + 50.f * j, -250.f + 50.f * i, 0.f });
		//		AddChild(wendigo);
		//	}
		//}

		//for (auto i = 0; i < 10; ++i)
		//{
		//	for (auto j = 0; j < 10; ++j)
		//	{
		//		auto fallen_shaman = object_manager->CreateActor<FallenShaman>(TAG{ "FallenShaman", object_manager->counter() });
		//		auto const& fallen_shaman_transform = CPTR_CAST<Transform>(fallen_shaman->FindComponent(TAG{ TRANSFORM, 0 }));
		//		fallen_shaman_transform->set_parent_flag(static_cast<uint8_t>(PARENT_FLAG::TRANSLATION));
		//		fallen_shaman_transform->set_local_translation(Vector3{ -250.f + 50.f * j, -1000.f + 50.f * i, 0.f });
		//		AddChild(fallen_shaman);
		//	}
		//}
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Andariel::Initialize" << std::endl;
	}
}

K::APTR K::Andariel::Clone() const
{
	return APTR{ new Andariel{ *this }, [](Andariel* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::Andariel::Serialize(InputMemoryStream& _imstream)
{
	for (auto& component : component_list_)
		component->Serialize(_imstream);

	for (auto& child : child_list_)
		child->Serialize(_imstream);
}

void K::Andariel::Serialize(OutputMemoryStream& _omstream)
{
	for (auto& component : component_list_)
		component->Serialize(_omstream);

	for (auto& child : child_list_)
		child->Serialize(_omstream);
}

K::Andariel::Andariel(Andariel const& _other) : MonsterActor(_other)
{
}

K::Andariel::Andariel(Andariel&& _other) noexcept : MonsterActor(std::move(_other))
{
}

void K::Andariel::_Finalize()
{
}

void K::Andariel::_Input(float _time)
{
	auto const& transform = CPTR_CAST<Transform>(FindComponent(TAG{ TRANSFORM, 0 }));
	auto const& animation_2d = CPTR_CAST<Animation2D>(FindComponent(TAG{ ANIMATION_2D, 0 }));
	auto const& navigator = CPTR_CAST<Navigator>(FindComponent(TAG{ NAVIGATOR, 0 }));

	switch (state_)
	{
	case K::ACTOR_STATE::ATTACK1:
	case K::ACTOR_STATE::ATTACK2:
	case K::ACTOR_STATE::GET_HIT:
	case K::ACTOR_STATE::DEAD:
	case K::ACTOR_STATE::DEATH:
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

			if (Vector3::Distance(position, target_position) <= 150.f)
			{
				set_state(ACTOR_STATE::SPECIAL_CAST);

				navigator->ClearPath();
			}
			else
				set_state(ACTOR_STATE::WALK);
		}
		break;
	}
}

void K::Andariel::_Update(float _time)
{
	auto const& transform = CPTR_CAST<Transform>(FindComponent(TAG{ TRANSFORM, 0 }));
	auto const& animation_2d = CPTR_CAST<Animation2D>(FindComponent(TAG{ ANIMATION_2D, 0 }));
	auto const& navigator = CPTR_CAST<Navigator>(FindComponent(TAG{ NAVIGATOR, 0 }));

	auto direction = navigator->direction();

	auto angle = DirectX::XMConvertToDegrees(acosf(-Vector3::UnitY.Dot(direction)));

	int dir_idx{};

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

	switch (state_)
	{
	case K::ACTOR_STATE::ATTACK1:
		animation_2d->SetCurrentClip("andariel_attack1", dir_idx);
		break;
	case K::ACTOR_STATE::ATTACK2:
		break;
	case K::ACTOR_STATE::GET_HIT:
		animation_2d->SetCurrentClip("andariel_get_hit", dir_idx);

		if (false == once_flag_array_.at(static_cast<int>(ACTOR_STATE::GET_HIT)) && 0 == animation_2d->frame_idx() % 6)
		{
			std::random_device r{};
			std::default_random_engine gen{ r() };
			std::uniform_int_distribution uniform_dist{ 1, 4 };
			auto number = uniform_dist(gen);

			AudioManager::singleton()->FindSoundEffect("andariel_gethit" + std::to_string(number))->Play(2.f, 0.f, 0.f);

			once_flag_array_.at(static_cast<int>(ACTOR_STATE::GET_HIT)) = true;
		}
		else if (0 != animation_2d->frame_idx() % 6)
			once_flag_array_.at(static_cast<int>(ACTOR_STATE::GET_HIT)) = false;
		break;
	case K::ACTOR_STATE::DEAD:
		break;
	case K::ACTOR_STATE::DEATH:
		animation_2d->SetCurrentClip("andariel_death", dir_idx);

		if (false == death_flag_)
		{
			auto const& object_manager = ObjectManager::singleton();
			auto andariel_death_overlay = object_manager->CreateActor<AndarielDeathOverlay>(TAG{ "AndarielDeathOverlay", 0 });
			auto const& ADO_transform = CPTR_CAST<Transform>(andariel_death_overlay->FindComponent(TAG{ TRANSFORM, 0 }));
			ADO_transform->set_local_translation(transform->world().Translation() + Vector3{ 0.f, 40.f, 0.f });
			auto const& ADO_animation_2d = CPTR_CAST<Animation2D>(andariel_death_overlay->FindComponent(TAG{ ANIMATION_2D, 0 }));
			ADO_animation_2d->SetCurrentClip("andariel_death(overlay)", dir_idx);
			layer()->AddActor(andariel_death_overlay);

			AudioManager::singleton()->FindSoundEffect("andariel_death")->Play(2.f, 0.f, 0.f);

			death_flag_ = true;
		}
		break;
	case K::ACTOR_STATE::NEUTRAL:
		animation_2d->SetCurrentClip("andariel_neutral", dir_idx);
		break;
	case K::ACTOR_STATE::RUN:
	case K::ACTOR_STATE::SPECIAL_CAST:
		animation_2d->SetCurrentClip("andariel_special_cast", dir_idx);

		if (false == once_flag_array_.at(static_cast<int>(ACTOR_STATE::SPECIAL_CAST)) && 0 == animation_2d->frame_idx() % 18)
		{
			AudioManager::singleton()->FindSoundEffect("andariel_castsmall")->Play(0.5f, 0.f, 0.f);

			auto position = transform->world().Translation();

			auto const& object_manager = ObjectManager::singleton();
			auto const& default_layer = WorldManager::singleton()->FindLayer(TAG{ "DefaultLayer", 1 });

			auto andariel_special_cast_overlay = object_manager->CreateActor<AndarielSpecialCastOverlay>(TAG{ "AndarielSpecialCastOverlay", 0 });
			auto const& ASCO_transform = CPTR_CAST<Transform>(andariel_special_cast_overlay->FindComponent(TAG{ TRANSFORM, 0 }));
			ASCO_transform->set_local_translation(position + Vector3{ 0.f, 20.f, 0.f });
			auto const& ASCO_animation_2d = CPTR_CAST<Animation2D>(andariel_special_cast_overlay->FindComponent(TAG{ ANIMATION_2D, 0 }));
			ASCO_animation_2d->SetCurrentClip("andariel_special_cast(overlay)", dir_idx);
			default_layer->AddActor(andariel_special_cast_overlay);

			for (auto i = -4; i < 5; ++i)
			{
				auto andariel_poisonball = object_manager->CreateActor<AndarielPoisonball>(TAG{ "AndarielPoisonball", 0 });
				APTR_CAST<AndarielPoisonball>(andariel_poisonball)->set_direction(Vector3::TransformNormal(direction, Matrix::CreateRotationZ(5.f * i)));
				auto const& AP_transform = CPTR_CAST<Transform>(andariel_poisonball->FindComponent(TAG{ TRANSFORM, 0 }));
				AP_transform->set_local_translation(position + Vector3{ 0.f, 40.f, 0.f });
				auto const& AP_animation_2d = CPTR_CAST<Animation2D>(andariel_poisonball->FindComponent(TAG{ ANIMATION_2D, 0 }));
				AP_animation_2d->SetCurrentClip("andariel_poisonball", -1);
				default_layer->AddActor(andariel_poisonball);
			}

			once_flag_array_.at(static_cast<int>(ACTOR_STATE::SPECIAL_CAST)) = true;
		}
		else if (0 != animation_2d->frame_idx() % 18)
			once_flag_array_.at(static_cast<int>(ACTOR_STATE::SPECIAL_CAST)) = false;
		break;
	case K::ACTOR_STATE::SPECIAL1:
		break;
	case K::ACTOR_STATE::WALK:
		animation_2d->SetCurrentClip("andariel_walk", dir_idx);

		if (false == once_flag_array_.at(static_cast<int>(ACTOR_STATE::WALK)) && 0 == animation_2d->frame_idx() % 12)
		{
			std::random_device r{};
			std::default_random_engine gen{ r() };
			std::uniform_int_distribution uniform_dist{ 1, 9 };
			auto number = uniform_dist(gen);

			AudioManager::singleton()->FindSoundEffect("andariel_stonehit" + std::to_string(number))->Play();

			once_flag_array_.at(static_cast<int>(ACTOR_STATE::WALK)) = true;
		}
		else if (0 != animation_2d->frame_idx() % 12)
			once_flag_array_.at(static_cast<int>(ACTOR_STATE::WALK)) = false;

		break;
	}
}
