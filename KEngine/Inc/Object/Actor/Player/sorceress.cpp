#include "KEngine.h"
#include "sorceress.h"

#include "Audio/audio_manager.h"
#include "Resource/resource_manager.h"
#include "Rendering/rendering_manager.h"
#include "input_manager.h"
#include "World/world_manager.h"
#include "World/layer.h"
#include "Object/object_manager.h"
#include "Object/Actor/camera_actor.h"
#include "Object/Actor/Missile/ice_cast.h"
#include "Object/Actor/Missile/ice_bolt.h"
#include "Object/Actor/Missile/ice_blast.h"
#include "Object/Actor/Missile/ice_orb.h"
#include "Object/Actor/Missile/frost_nova.h"
#include "Object/Actor/Missile/frozen_armor.h"
#include "Object/Component/transform.h"
#include "Object/Component/material.h"
#include "Object/Component/renderer.h"
#include "Object/Component/animation_2d.h"
#include "Object/Component/Collider/collider_aabb.h"
#include "Object/Component/navigator.h"

void K::Sorceress::Initialize()
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
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("sorceress_attack1");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("sorceress_attack2");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("sorceress_get_hit");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("sorceress_neutral");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("sorceress_run");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("sorceress_special_cast");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("sorceress_special1");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("sorceress_walk");
		CPTR_CAST<Animation2D>(animation_2d)->SetCurrentClip("sorceress_neutral", 0);
		CPTR_CAST<Animation2D>(animation_2d)->SetDefaultClip("sorceress_neutral", 0);
		AddComponent(animation_2d);

		auto collider = object_manager->CreateComponent<ColliderAABB>(TAG{ COLLIDER, 0 });
		CPTR_CAST<ColliderAABB>(collider)->set_relative_info(AABB{ Vector3::Zero, Vector3{ 15.f, 35.f, 0.f } });
		CPTR_CAST<ColliderAABB>(collider)->set_owner_type(OWNER_TYPE::PLAYER);
		AddComponent(collider);

		auto navigator = object_manager->CreateComponent<Navigator>(TAG{ NAVIGATOR, 0 });
		CPTR_CAST<Navigator>(navigator)->set_speed(320.f);
		AddComponent(navigator);

		set_state(ACTOR_STATE::NEUTRAL);

		set_hp(30000.f);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Sorceress::Initialize" << std::endl;
	}
}

K::APTR K::Sorceress::Clone() const
{
	return APTR{ new Sorceress{ *this }, [](Sorceress* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::Sorceress::Serialize(InputMemoryStream& _imstream)
{
	for (auto& component : component_list_)
		component->Serialize(_imstream);

	for (auto& child : child_list_)
		child->Serialize(_imstream);
}

void K::Sorceress::Serialize(OutputMemoryStream& _omstream)
{
	for (auto& component : component_list_)
		component->Serialize(_omstream);

	for (auto& child : child_list_)
		child->Serialize(_omstream);
}

void K::Sorceress::_Input(float _time)
{
	auto const& audio_manager = AudioManager::singleton();
	auto const& input_manager = InputManager::singleton();
	auto const& object_manager = ObjectManager::singleton();
	auto const& animation_2d = CPTR_CAST<Animation2D>(FindComponent(TAG{ ANIMATION_2D, 0 }));

	auto position = CPTR_CAST<Transform>(FindComponent(TAG{ TRANSFORM, 0 }))->world().Translation();
	auto mouse_world_position = input_manager->mouse_world_position();

	//if (input_manager->KeyDown("Space"))
	//{
	//	std::cout << "Position: " << static_cast<int>(position.x) << ", " << static_cast<int>(position.y) << std::endl;
	//	std::cout << "Mouse Position: " << static_cast<int>(mouse_world_position.x) << ", " << static_cast<int>(mouse_world_position.y) << std::endl;
	//}

	auto const& navigator = CPTR_CAST<Navigator>(FindComponent(TAG{ NAVIGATOR, 0 }));

	if (input_manager->KeyPressed("LButton"))
		navigator->Route(position, mouse_world_position);

	if (input_manager->KeyDown("R"))
		run_flag_ ^= true;

	if (navigator->move_flag())
	{
		if (run_flag_)
		{
			navigator->set_speed(400.f);
			set_state(ACTOR_STATE::RUN);
		}
		else
		{
			navigator->set_speed(200.f);
			set_state(ACTOR_STATE::WALK);
		}
	}

	if (true == focus_flag_)
	{
		auto const& camera = WorldManager::singleton()->FindCamera(TAG{ DEFAULT_CAMERA, 0 });
		auto const& camera_transform = CPTR_CAST<Transform>(camera->FindComponent(TAG{ TRANSFORM, 0 }));
		camera_transform->set_local_translation(position);
		camera_transform->Update(_time);
	}

	auto direction = navigator->direction();

	auto angle = DirectX::XMConvertToDegrees(acosf(-Vector3::UnitY.Dot(direction)));

	int dir_idx{};

	if (direction.x < 0.f)
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

	if (ACTOR_STATE::NEUTRAL == state())
	{
		if (input_manager->KeyDown("1"))
		{
			audio_manager->FindSoundEffect("ice_cast")->Play();

			auto const& ui_layer = WorldManager::singleton()->FindLayer(TAG{ "UILayer", 2 });

			auto ice_cast = object_manager->CreateActor<IceCast>(TAG{ "IceCast", 0 });
			auto const& ice_cast_transform = CPTR_CAST<Transform>(ice_cast->FindComponent(TAG{ TRANSFORM, 0 }));
			auto const& ice_cast_animation_2d = CPTR_CAST<Animation2D>(ice_cast->FindComponent(TAG{ ANIMATION_2D, 0 }));

			ice_cast_transform->set_local_translation(position + Vector3{ 0.f, 40.f, 0.f });
			ice_cast_animation_2d->SetCurrentClip("ice_cast1", -1);
			ui_layer->AddActor(ice_cast);

			direction = mouse_world_position - position;
			direction.Normalize();
			navigator->set_direction(direction);

			angle = DirectX::XMConvertToDegrees(acosf(-Vector3::UnitY.Dot(direction)));

			if (direction.x < 0.f)
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

			animation_2d->set_callback([position, direction, dir_idx]() {
				auto ice_bolt = ObjectManager::singleton()->CreateActor<IceBolt>(TAG{ "IceBolt", 0 });
				auto const& ice_bolt_transform = CPTR_CAST<Transform>(ice_bolt->FindComponent(TAG{ TRANSFORM, 0 }));
				auto const& ice_bolt_animation_2d = CPTR_CAST<Animation2D>(ice_bolt->FindComponent(TAG{ ANIMATION_2D, 0 }));

				APTR_CAST<IceBolt>(ice_bolt)->set_direction(direction);
				ice_bolt_transform->set_local_translation(position);

				std::random_device r{};
				std::default_random_engine gen{ r() };
				std::uniform_int_distribution uniform_dist{ 1, 3 };
				auto number = uniform_dist(gen);

				std::string skill_name = "ice_bolt" + std::to_string(number);

				AudioManager::singleton()->FindSoundEffect(skill_name)->Play();
				ice_bolt_animation_2d->SetCurrentClip("ice_bolt", dir_idx);

				auto const& lambda_layer = WorldManager::singleton()->FindLayer(TAG{ "DefaultLayer", 1 });

				lambda_layer->AddActor(ice_bolt);
			});

			set_state(ACTOR_STATE::SPECIAL_CAST);
		}

		if (input_manager->KeyDown("2"))
		{
			audio_manager->FindSoundEffect("ice_cast")->Play();

			auto const& ui_layer = WorldManager::singleton()->FindLayer(TAG{ "UILayer", 2 });

			auto ice_cast = object_manager->CreateActor<IceCast>(TAG{ "IceCast", 0 });
			auto const& ice_cast_transform = CPTR_CAST<Transform>(ice_cast->FindComponent(TAG{ TRANSFORM, 0 }));
			auto const& ice_cast_animation_2d = CPTR_CAST<Animation2D>(ice_cast->FindComponent(TAG{ ANIMATION_2D, 0 }));

			ice_cast_transform->set_local_translation(position + Vector3{ 0.f, 40.f, 0.f });
			ice_cast_animation_2d->SetCurrentClip("ice_cast1", -1);
			ui_layer->AddActor(ice_cast);

			direction = mouse_world_position - position;
			direction.Normalize();
			navigator->set_direction(direction);

			angle = DirectX::XMConvertToDegrees(acosf(-Vector3::UnitY.Dot(direction)));

			if (direction.x < 0.f)
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

			animation_2d->set_callback([position, direction, dir_idx]() {
				auto ice_blast = ObjectManager::singleton()->CreateActor<IceBlast>(TAG{ "IceBlast", 0 });
				auto const& ice_blast_transform = CPTR_CAST<Transform>(ice_blast->FindComponent(TAG{ TRANSFORM, 0 }));
				auto const& ice_blast_animation_2d = CPTR_CAST<Animation2D>(ice_blast->FindComponent(TAG{ ANIMATION_2D, 0 }));

				APTR_CAST<IceBlast>(ice_blast)->set_direction(direction);
				ice_blast_transform->set_local_translation(position);

				std::random_device r{};
				std::default_random_engine gen{ r() };
				std::uniform_int_distribution uniform_dist{ 1, 3 };
				auto number = uniform_dist(gen);

				std::string skill_name = "ice_blast" + std::to_string(number);

				AudioManager::singleton()->FindSoundEffect(skill_name)->Play();
				ice_blast_animation_2d->SetCurrentClip("ice_blast", dir_idx);

				auto const& lambda_layer = WorldManager::singleton()->FindLayer(TAG{ "DefaultLayer", 1 });

				lambda_layer->AddActor(ice_blast);
			});

			set_state(ACTOR_STATE::SPECIAL_CAST);
		}

		if (input_manager->KeyDown("3"))
		{
			audio_manager->FindSoundEffect("ice_cast")->Play();

			auto const& ui_layer = WorldManager::singleton()->FindLayer(TAG{ "UILayer", 2 });

			auto ice_cast = object_manager->CreateActor<IceCast>(TAG{ "IceCast", 0 });
			auto const& ice_cast_transform = CPTR_CAST<Transform>(ice_cast->FindComponent(TAG{ TRANSFORM, 0 }));
			auto const& ice_cast_animation_2d = CPTR_CAST<Animation2D>(ice_cast->FindComponent(TAG{ ANIMATION_2D, 0 }));

			ice_cast_transform->set_local_translation(position);
			ice_cast_animation_2d->SetCurrentClip("ice_cast3", -1);
			ui_layer->AddActor(ice_cast);

			direction = mouse_world_position - position;
			direction.Normalize();
			navigator->set_direction(direction);

			angle = DirectX::XMConvertToDegrees(acosf(-Vector3::UnitY.Dot(direction)));

			if (direction.x < 0.f)
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

			animation_2d->set_callback([position, direction, dir_idx]() {
				auto ice_orb = ObjectManager::singleton()->CreateActor<IceOrb>(TAG{ "IceOrb", 0 });
				auto const& ice_orb_transform = CPTR_CAST<Transform>(ice_orb->FindComponent(TAG{ TRANSFORM, 0 }));
				auto const& ice_orb_animation_2d = CPTR_CAST<Animation2D>(ice_orb->FindComponent(TAG{ ANIMATION_2D, 0 }));

				APTR_CAST<IceOrb>(ice_orb)->set_direction(direction);
				ice_orb_transform->set_local_translation(position);

				std::string skill_name = "ice_orb";

				AudioManager::singleton()->FindSoundEffect(skill_name)->Play();
				ice_orb_animation_2d->SetCurrentClip("ice_orb", -1);

				auto const& lambda_layer = WorldManager::singleton()->FindLayer(TAG{ "DefaultLayer", 1 });

				lambda_layer->AddActor(ice_orb);
			});

			set_state(ACTOR_STATE::SPECIAL_CAST);
		}

		if (input_manager->KeyDown("4"))
		{
			audio_manager->FindSoundEffect("ice_cast")->Play();

			auto const& ui_layer = WorldManager::singleton()->FindLayer(TAG{ "UILayer", 2 });

			auto ice_cast = object_manager->CreateActor<IceCast>(TAG{ "IceCast", 0 });
			auto const& ice_cast_transform = CPTR_CAST<Transform>(ice_cast->FindComponent(TAG{ TRANSFORM, 0 }));
			auto const& ice_cast_animation_2d = CPTR_CAST<Animation2D>(ice_cast->FindComponent(TAG{ ANIMATION_2D, 0 }));

			ice_cast_transform->set_local_translation(position);
			ice_cast_animation_2d->SetCurrentClip("ice_cast2", -1);
			ui_layer->AddActor(ice_cast);

			direction = mouse_world_position - position;
			direction.Normalize();
			navigator->set_direction(direction);

			angle = DirectX::XMConvertToDegrees(acosf(-Vector3::UnitY.Dot(direction)));

			animation_2d->set_callback([position]() {
				auto const& lambda_layer = WorldManager::singleton()->FindLayer(TAG{ "DefaultLayer", 1 });

				std::string skill_name = "frost_nova";
				AudioManager::singleton()->FindSoundEffect(skill_name)->Play();

				for (auto i = 0; i < 64; ++i)
				{
					auto frost_nova = ObjectManager::singleton()->CreateActor<FrostNova>(TAG{ "FrostNova", 0 });
					auto const& frost_nova_transform = CPTR_CAST<Transform>(frost_nova->FindComponent(TAG{ TRANSFORM, 0 }));
					auto const& frost_nova_animation_2d = CPTR_CAST<Animation2D>(frost_nova->FindComponent(TAG{ ANIMATION_2D, 0 }));

					APTR_CAST<FrostNova>(frost_nova)->set_direction(Vector3::TransformNormal(Vector3::UnitX, Matrix::CreateRotationZ(5.625f * i))); 

					frost_nova_transform->set_local_translation(position + Vector3{ 0.f, -20.f, 0.f });

					frost_nova_animation_2d->SetCurrentClip("frost_nova", i / 4);

					lambda_layer->AddActor(frost_nova);
				}
			});

			set_state(ACTOR_STATE::SPECIAL_CAST);
		}

		if (input_manager->KeyDown("5"))
		{
			if (false == frozen_armor_flag_)
			{
				audio_manager->FindSoundEffect("frozen_armor")->Play();

				auto const& ui_layer = WorldManager::singleton()->FindLayer(TAG{ "UILayer", 2 });

				auto ice_cast = object_manager->CreateActor<IceCast>(TAG{ "IceCast", 0 });
				auto const& ice_cast_transform = CPTR_CAST<Transform>(ice_cast->FindComponent(TAG{ TRANSFORM, 0 }));
				auto const& ice_cast_animation_2d = CPTR_CAST<Animation2D>(ice_cast->FindComponent(TAG{ ANIMATION_2D, 0 }));

				ice_cast_transform->set_local_translation(position + Vector3{ 0.f, 40.f, 0.f });
				ice_cast_animation_2d->SetCurrentClip("ice_cast1", -1);
				ui_layer->AddActor(ice_cast);

				direction = mouse_world_position - position;
				direction.Normalize();
				navigator->set_direction(direction);

				auto const& lambda_layer = WorldManager::singleton()->FindLayer(TAG{ "DefaultLayer", 1 });

				auto frozen_armor = object_manager->CreateActor<FrozenArmor>(TAG{ "FrozenArmor", 0 });
				APTR_CAST<FrozenArmor>(frozen_armor)->set_target(shared_from_this());
				auto const& frozen_armor_transform = CPTR_CAST<Transform>(frozen_armor->FindComponent(TAG{ TRANSFORM, 0 }));
				auto const& frozen_armor_animation_2d = CPTR_CAST<Animation2D>(frozen_armor->FindComponent(TAG{ ANIMATION_2D, 0 }));

				frozen_armor_transform->set_local_translation(position);
				
				lambda_layer->AddActor(frozen_armor);

				animation_2d->set_callback([]() {});

				frozen_armor_flag_ = true;
				set_state(ACTOR_STATE::SPECIAL_CAST);
			}
		}

		if (input_manager->KeyDown("6"))
		{
			audio_manager->FindSoundEffect("teleport")->Play();

			auto const& ui_layer = WorldManager::singleton()->FindLayer(TAG{ "UILayer", 2 });

			auto ice_cast = object_manager->CreateActor<IceCast>(TAG{ "IceCast", 0 });
			auto const& ice_cast_transform = CPTR_CAST<Transform>(ice_cast->FindComponent(TAG{ TRANSFORM, 0 }));
			auto const& ice_cast_animation_2d = CPTR_CAST<Animation2D>(ice_cast->FindComponent(TAG{ ANIMATION_2D, 0 }));

			ice_cast_transform->set_local_translation(position + Vector3{ 0.f, -40.f, 0.f });
			ice_cast_animation_2d->SetCurrentClip("teleport", -1);
			ui_layer->AddActor(ice_cast);

			direction = mouse_world_position - position;
			direction.Normalize();
			navigator->set_direction(direction);

			animation_2d->set_callback([this, direction, mouse_world_position]() {
				auto const& transform = CPTR_CAST<Transform>(FindComponent(TAG{ TRANSFORM, 0 }));

				auto position = transform->world().Translation();

				auto distance = Vector3::Distance(position, mouse_world_position);
				distance = std::clamp(distance, 0.f, 600.f);

				transform->set_local_translation(transform->local_translation() + direction * distance);
			});

			set_state(ACTOR_STATE::SPECIAL_CAST);
		}
	}

	switch (state_)
	{
	case K::ACTOR_STATE::ATTACK1:
		animation_2d->SetCurrentClip("sorceress_attack1", dir_idx);
		break;
	case K::ACTOR_STATE::ATTACK2:
		animation_2d->SetCurrentClip("sorceress_attack2", dir_idx);
		break;
	case K::ACTOR_STATE::GET_HIT:
		animation_2d->SetCurrentClip("sorceress_get_hit", dir_idx);

		if (false == once_flag_array_.at(static_cast<int>(ACTOR_STATE::GET_HIT)) && 0 == animation_2d->frame_idx() % 8)
		{
			std::random_device r{};
			std::default_random_engine gen{ r() };
			std::uniform_int_distribution uniform_dist{ 1, 5 };
			auto number = uniform_dist(gen);

			AudioManager::singleton()->FindSoundEffect("sorceress_gethit" + std::to_string(number))->Play(2.f, 0.f, 0.f);

			once_flag_array_.at(static_cast<int>(ACTOR_STATE::GET_HIT)) = true;
		}
		else if (0 != animation_2d->frame_idx() % 8)
			once_flag_array_.at(static_cast<int>(ACTOR_STATE::GET_HIT)) = false;

		break;
	case K::ACTOR_STATE::DEAD:
		break;
	case K::ACTOR_STATE::DEATH:
		break;
	case K::ACTOR_STATE::NEUTRAL:
		animation_2d->SetCurrentClip("sorceress_neutral", dir_idx);
		break;
	case K::ACTOR_STATE::RUN:
		animation_2d->SetCurrentClip("sorceress_run", dir_idx);

		if (false == once_flag_array_.at(static_cast<int>(ACTOR_STATE::RUN)) && 0 == animation_2d->frame_idx() % 8)
		{
			AudioManager::singleton()->FindSoundEffect("sorceress_run")->Play(2.f, 0.f, 0.f);

			once_flag_array_.at(static_cast<int>(ACTOR_STATE::RUN)) = true;
		}
		else if (0 != animation_2d->frame_idx() % 8)
			once_flag_array_.at(static_cast<int>(ACTOR_STATE::RUN)) = false;
		break;
	case K::ACTOR_STATE::SPECIAL_CAST:
		animation_2d->SetCurrentClip("sorceress_special_cast", dir_idx);
		break;
	case K::ACTOR_STATE::SPECIAL1:
		animation_2d->SetCurrentClip("sorceress_special1", dir_idx);
		break;
	case K::ACTOR_STATE::WALK:
		animation_2d->SetCurrentClip("sorceress_walk", dir_idx);

		if (false == once_flag_array_.at(static_cast<int>(ACTOR_STATE::WALK)) && 0 == animation_2d->frame_idx() % 8)
		{
			AudioManager::singleton()->FindSoundEffect("sorceress_walk")->Play(2.f, 0.f, 0.f);

			once_flag_array_.at(static_cast<int>(ACTOR_STATE::WALK)) = true;
		}
		else if (0 != animation_2d->frame_idx() % 8)
			once_flag_array_.at(static_cast<int>(ACTOR_STATE::WALK)) = false;

		break;
	}
}

void K::Sorceress::set_frozen_armor_flag(bool _flag)
{
	frozen_armor_flag_ = _flag;
}

K::Sorceress::Sorceress(Sorceress const& _other) : PlayerActor(_other)
{
}

K::Sorceress::Sorceress(Sorceress&& _other) noexcept : PlayerActor(std::move(_other))
{
}

void K::Sorceress::_Finalize()
{
}
