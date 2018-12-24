#include "KEngine.h"
#include "andariel.h"

#include "Resource/resource_manager.h"
#include "Rendering/rendering_manager.h"
#include "Object/object_manager.h"
#include "Object/Component/transform.h"
#include "Object/Component/material.h"
#include "Object/Component/renderer.h"
#include "Object/Component/animation_2d.h"
#include "Object/Component/Collider/collider_aabb.h"

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
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_attack1");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_death");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_death(overlay)");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_get_hit");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_neutral");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_walk");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_special_cast");
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("andariel_special_cast(overlay)");
		CPTR_CAST<Animation2D>(animation_2d)->SetCurrentClip("andariel_walk", 0);
		CPTR_CAST<Animation2D>(animation_2d)->SetDefaultClip("andariel_walk", 0);
		AddComponent(animation_2d);

		auto collider = object_manager->CreateComponent<ColliderAABB>(TAG{ COLLIDER, 0 });
		AddComponent(collider);
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
