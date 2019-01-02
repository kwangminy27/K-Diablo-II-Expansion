#include "KEngine.h"
#include "fallen_shaman_shadow.h"

#include "Resource/resource_manager.h"
#include "Rendering/rendering_manager.h"
#include "input_manager.h"
#include "World/world_manager.h"
#include "World/layer.h"
#include "Object/object_manager.h"
#include "Object/Actor/Monster/andariel.h"
#include "Object/Component/transform.h"
#include "Object/Component/material.h"
#include "Object/Component/renderer.h"
#include "Object/Component/animation_2d.h"

void K::FallenShamanShadow::Initialize()
{
	try
	{
		auto const& object_manager = ObjectManager::singleton();

		auto transform = object_manager->CreateComponent<Transform>(TAG{ TRANSFORM, 0 });
		AddComponent(transform);

		auto material = object_manager->CreateComponent<Material>(TAG{ MATERIAL, 0 });
		CPTR_CAST<Material>(material)->SetSampler(LINEAR_SAMPLER, 0, 0, 0);
		MaterialConstantBuffer Material_CB{};
		Material_CB.diffuse = Vector4{ 0.f, 0.f, 0.f, 0.5f };
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
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("fallen_shaman_get_hit");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("fallen_shaman_neutral");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("fallen_shaman_walk");
		CPTR_CAST<Animation2D>(animation_2d)->SetCurrentClip("fallen_shaman_neutral", 0);
		CPTR_CAST<Animation2D>(animation_2d)->SetDefaultClip("fallen_shaman_neutral", 0);
		AddComponent(animation_2d);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "FallenShamanShadow::Initialize" << std::endl;
	}
}

K::APTR K::FallenShamanShadow::Clone() const
{
	return APTR{ new FallenShamanShadow{ *this }, [](FallenShamanShadow* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::FallenShamanShadow::Serialize(InputMemoryStream& _imstream)
{
}

void K::FallenShamanShadow::Serialize(OutputMemoryStream& _omstream)
{
}

void K::FallenShamanShadow::_Render(float _time)
{
	auto const& animation_2d = CPTR_CAST<Animation2D>(FindComponent(TAG{ ANIMATION_2D, 0 }));

	auto _parent = parent();
	set_state(_parent->state());

	auto dir_idx = _parent->shadow_dir_idx();

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
		break;
	case K::ACTOR_STATE::DEATH:
		set_tag_state(TAG_STATE::DEAD);
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

K::FallenShamanShadow::FallenShamanShadow(FallenShamanShadow const& _other) : MonsterActor(_other)
{
}

K::FallenShamanShadow::FallenShamanShadow(FallenShamanShadow&& _other) noexcept : MonsterActor(std::move(_other))
{
}

void K::FallenShamanShadow::_Finalize()
{
}
