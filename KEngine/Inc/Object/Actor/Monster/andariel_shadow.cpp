#include "KEngine.h"
#include "andariel_shadow.h"

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

void K::AndarielShadow::Initialize()
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
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_attack1");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_get_hit");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_neutral");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_walk");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_special_cast");
		CPTR_CAST<Animation2D>(animation_2d)->SetCurrentClip("andariel_walk", 0);
		CPTR_CAST<Animation2D>(animation_2d)->SetDefaultClip("andariel_walk", 0);
		AddComponent(animation_2d);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "AndarielShadow::Initialize" << std::endl;
	}
}

K::APTR K::AndarielShadow::Clone() const
{
	return APTR{ new AndarielShadow{ *this }, [](AndarielShadow* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::AndarielShadow::Serialize(InputMemoryStream& _imstream)
{
}

void K::AndarielShadow::Serialize(OutputMemoryStream& _omstream)
{
}

void K::AndarielShadow::_Render(float _time)
{
	auto const& animation_2d = CPTR_CAST<Animation2D>(FindComponent(TAG{ ANIMATION_2D, 0 }));

	auto _parent = parent();
	set_state(_parent->state());

	auto dir_idx = _parent->shadow_dir_idx();

	switch (state_)
	{
	case K::ACTOR_STATE::ATTACK1:
		animation_2d->SetCurrentClip("andariel_attack1", dir_idx);
		break;
	case K::ACTOR_STATE::ATTACK2:
		break;
	case K::ACTOR_STATE::GET_HIT:
		animation_2d->SetCurrentClip("andariel_get_hit", dir_idx);
		break;
	case K::ACTOR_STATE::DEAD:
		break;
	case K::ACTOR_STATE::DEATH:
		set_tag_state(TAG_STATE::DEAD);
		break;
	case K::ACTOR_STATE::NEUTRAL:
		animation_2d->SetCurrentClip("andariel_neutral", dir_idx);
		break;
	case K::ACTOR_STATE::RUN:
		break;
	case K::ACTOR_STATE::SPECIAL_CAST:
		animation_2d->SetCurrentClip("andariel_special_cast", dir_idx);
		break;
	case K::ACTOR_STATE::SPECIAL1:
		break;
	case K::ACTOR_STATE::WALK:
		animation_2d->SetCurrentClip("andariel_walk", dir_idx);
		break;
	}
}

K::AndarielShadow::AndarielShadow(AndarielShadow const& _other) : MonsterActor(_other)
{
}

K::AndarielShadow::AndarielShadow(AndarielShadow&& _other) noexcept : MonsterActor(std::move(_other))
{
}

void K::AndarielShadow::_Finalize()
{
}
