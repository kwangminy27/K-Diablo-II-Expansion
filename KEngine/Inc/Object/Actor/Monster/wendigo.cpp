#include "KEngine.h"
#include "wendigo.h"
#include "wendigo_shadow.h"

#include "Audio/audio_manager.h"
#include "Resource/resource_manager.h"
#include "Rendering/rendering_manager.h"
#include "Object/object_manager.h"
#include "Object/Component/transform.h"
#include "Object/Component/material.h"
#include "Object/Component/renderer.h"
#include "Object/Component/animation_2d.h"
#include "Object/Component/Collider/collider_aabb.h"
#include "Object/Component/Collider/collider_circle.h"
#include "Object/Component/navigator.h"

void K::Wendigo::Initialize()
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
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("wendigo_attack1");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("wendigo_attack2");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("wendigo_dead");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("wendigo_death");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("wendigo_get_hit");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("wendigo_neutral");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("wendigo_walk");
		CPTR_CAST<Animation2D>(animation_2d)->SetCurrentClip("wendigo_neutral", 0);
		CPTR_CAST<Animation2D>(animation_2d)->SetDefaultClip("wendigo_neutral", 0);
		AddComponent(animation_2d);

		auto collider = object_manager->CreateComponent<ColliderAABB>(TAG{ COLLIDER, 0 });
		CPTR_CAST<ColliderAABB>(collider)->set_relative_info(AABB{ Vector3::Zero, Vector3{ 40.f, 45.f, 0.f } });
		CPTR_CAST<ColliderAABB>(collider)->set_owner_type(OWNER_TYPE::MONSTER);
		AddComponent(collider);

		auto view_range = object_manager->CreateComponent<ColliderCircle>(TAG{ COLLIDER, 1 });
		CPTR_CAST<ColliderCircle>(view_range)->set_relative_info(Circle{ Vector3::Zero, 400.f });
		CPTR_CAST<ColliderAABB>(view_range)->set_owner_type(OWNER_TYPE::VIEW);
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
		CPTR_CAST<Navigator>(navigator)->set_speed(200.f);
		AddComponent(navigator);

		auto shadow = object_manager->CreateActor<WendigoShadow>(TAG{ "WendigoShadow", 0 });
		auto const& shadow_transform = CPTR_CAST<Transform>(shadow->FindComponent(TAG{ TRANSFORM, 0 }));
		shadow_transform->set_parent_flag(static_cast<uint8_t>(PARENT_FLAG::TRANSLATION));
		shadow_transform->set_local_translation(Vector3{ 15.f, 10.f, 0.f });
		shadow_transform->set_local_rotation(Quaternion::CreateFromYawPitchRoll(0.f, 0.f, -30.f));
		AddChild(shadow);

		set_state(ACTOR_STATE::NEUTRAL);

		set_hp(100.f);
		set_speed(200.f);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Wendigo::Initialize" << std::endl;
	}
}

K::APTR K::Wendigo::Clone() const
{
	return APTR{ new Wendigo{ *this }, [](Wendigo* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::Wendigo::Serialize(InputMemoryStream& _imstream)
{
	for (auto& component : component_list_)
		component->Serialize(_imstream);

	//for (auto& child : child_list_)
	//	child->Serialize(_imstream);
}

void K::Wendigo::Serialize(OutputMemoryStream& _omstream)
{
	for (auto& component : component_list_)
		component->Serialize(_omstream);

	//for (auto& child : child_list_)
	//	child->Serialize(_omstream);
}

K::Wendigo::Wendigo(Wendigo const& _other) : MonsterActor(_other)
{
}

K::Wendigo::Wendigo(Wendigo&& _other) noexcept : MonsterActor(std::move(_other))
{
}

void K::Wendigo::_Finalize()
{
}

void K::Wendigo::_Input(float _time)
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
			else if (distance >= 450.f)
				target_.reset();
			else
				set_state(ACTOR_STATE::WALK);
		}
		break;
	}
}

void K::Wendigo::_Update(float _time)
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

	shadow_dir_idx_ = dir_idx;

	switch (state_)
	{
	case K::ACTOR_STATE::ATTACK1:
		animation_2d->SetCurrentClip("wendigo_attack1", dir_idx);

		if (false == once_flag_array_.at(static_cast<int>(ACTOR_STATE::ATTACK1)) && 0 == animation_2d->frame_idx() % 12)
		{
			std::random_device r{};
			std::default_random_engine gen{ r() };
			std::uniform_int_distribution uniform_dist{ 1, 3 };
			auto number = uniform_dist(gen);

			AudioManager::singleton()->FindSoundEffect("wendigo_attack" + std::to_string(number))->Play();

			once_flag_array_.at(static_cast<int>(ACTOR_STATE::ATTACK1)) = true;
		}
		else if (0 != animation_2d->frame_idx() % 12)
			once_flag_array_.at(static_cast<int>(ACTOR_STATE::ATTACK1)) = false;

		break;
	case K::ACTOR_STATE::ATTACK2:
		animation_2d->SetCurrentClip("wendigo_attack2", dir_idx);
		break;
	case K::ACTOR_STATE::GET_HIT:
		animation_2d->SetCurrentClip("wendigo_get_hit", dir_idx);

		if (false == once_flag_array_.at(static_cast<int>(ACTOR_STATE::GET_HIT)) && 0 == animation_2d->frame_idx() % 6)
		{
			std::random_device r{};
			std::default_random_engine gen{ r() };
			std::uniform_int_distribution uniform_dist{ 1, 4 };
			auto number = uniform_dist(gen);

			AudioManager::singleton()->FindSoundEffect("wendigo_gethit" + std::to_string(number))->Play();

			once_flag_array_.at(static_cast<int>(ACTOR_STATE::GET_HIT)) = true;
		}
		else if (0 != animation_2d->frame_idx() % 6)
			once_flag_array_.at(static_cast<int>(ACTOR_STATE::GET_HIT)) = false;

		break;
	case K::ACTOR_STATE::DEAD:
		animation_2d->SetCurrentClip("wendigo_dead", dir_idx);
		break;
	case K::ACTOR_STATE::DEATH:
		animation_2d->SetCurrentClip("wendigo_death", dir_idx);

		if (false == once_flag_array_.at(static_cast<int>(ACTOR_STATE::DEATH)) && 0 == animation_2d->frame_idx() % 18)
		{
			std::random_device r{};
			std::default_random_engine gen{ r() };
			std::uniform_int_distribution uniform_dist{ 1, 4 };
			auto number = uniform_dist(gen);

			AudioManager::singleton()->FindSoundEffect("wendigo_death" + std::to_string(number))->Play();

			once_flag_array_.at(static_cast<int>(ACTOR_STATE::DEATH)) = true;
		}
		else if (0 != animation_2d->frame_idx() % 18)
			once_flag_array_.at(static_cast<int>(ACTOR_STATE::DEATH)) = false;

		break;
	case K::ACTOR_STATE::NEUTRAL:
		animation_2d->SetCurrentClip("wendigo_neutral", dir_idx);
		break;
	case K::ACTOR_STATE::RUN:
	case K::ACTOR_STATE::SPECIAL_CAST:
	case K::ACTOR_STATE::SPECIAL1:
		break;
	case K::ACTOR_STATE::WALK:
		animation_2d->SetCurrentClip("wendigo_walk", dir_idx);
		break;
	}
}

void K::Wendigo::_Render(float _time)
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
