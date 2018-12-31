#include "KEngine.h"
#include "fallen_shaman.h"

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
		CPTR_CAST<ColliderAABB>(collider)->set_relative_info(AABB{ Vector3::Zero, Vector3{ 17.f, 48.f, 0.f } });
		AddComponent(collider);

		auto view_range = object_manager->CreateComponent<ColliderCircle>(TAG{ COLLIDER, 1 });
		CPTR_CAST<ColliderCircle>(view_range)->set_relative_info(Circle{ Vector3::Zero, 600.f });
		CPTR_CAST<ColliderCircle>(view_range)->AddCallback([this](Collider* _src, Collider* _dest, float _time) {
			if (_src->owner()->tag().first == "Sorceress")
				set_target(_src->owner());
			else if (_dest->owner()->tag().first == "Sorceress")
				set_target(_dest->owner());
		}, COLLISION_CALLBACK_TYPE::ENTER);
		CPTR_CAST<ColliderCircle>(view_range)->AddCallback([this](Collider* _src, Collider* _dest, float _time) {
			if (target_.expired())
				return;

			auto caching_target = target();

			if (_src->tag() == TAG{ COLLIDER, 0 } && caching_target == _src->owner())
				set_target(nullptr);

			if (_dest->tag() == TAG{ COLLIDER, 0 } && caching_target == _dest->owner())
				set_target(nullptr);
		}, COLLISION_CALLBACK_TYPE::LEAVE);
		AddComponent(view_range);

		auto navigator = object_manager->CreateComponent<Navigator>(TAG{ NAVIGATOR, 0 });
		CPTR_CAST<Navigator>(navigator)->set_speed(100.f);
		AddComponent(navigator);

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

	for (auto& child : child_list_)
		child->Serialize(_imstream);
}

void K::FallenShaman::Serialize(OutputMemoryStream& _omstream)
{
	for (auto& component : component_list_)
		component->Serialize(_omstream);

	for (auto& child : child_list_)
		child->Serialize(_omstream);
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
		animation_2d->SetCurrentClip("fallen_shaman_attack1", dir_idx);
		break;
	case K::ACTOR_STATE::ATTACK2:
		animation_2d->SetCurrentClip("fallen_shaman_attack2", dir_idx);
		break;
	case K::ACTOR_STATE::GET_HIT:
		animation_2d->SetCurrentClip("fallen_shaman_get_hit", dir_idx);
		break;
	case K::ACTOR_STATE::DEAD:
		animation_2d->SetCurrentClip("fallen_shaman_dead", dir_idx);
		break;
	case K::ACTOR_STATE::DEATH:
		animation_2d->SetCurrentClip("fallen_shaman_death", dir_idx);
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
}
