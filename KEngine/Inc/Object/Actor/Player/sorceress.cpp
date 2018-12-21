#include "KEngine.h"
#include "sorceress.h"

#include "Resource/resource_manager.h"
#include "Rendering/rendering_manager.h"
#include "input_manager.h"
#include "World/world_manager.h"
#include "Object/object_manager.h"
#include "Object/Actor/camera_actor.h"
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
		CPTR_CAST<Animation2D>(animation_2d)->SetCurrentClip("sorceress_walk");
		CPTR_CAST<Animation2D>(animation_2d)->SetDefaultClip("sorceress_walk");
		AddComponent(animation_2d);

		auto collider = object_manager->CreateComponent<ColliderAABB>(TAG{ COLLIDER, 0 });
		AddComponent(collider);

		auto navigator = object_manager->CreateComponent<Navigator>(TAG{ NAVIGATOR, 0 });
		CPTR_CAST<Navigator>(navigator)->set_speed(400.f);
		AddComponent(navigator);
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
	auto const& input_manager = InputManager::singleton();

	if (input_manager->KeyDown("LButton"))
	{
		auto position = CPTR_CAST<Transform>(FindComponent(TAG{ TRANSFORM, 0 }))->world().Translation();
		auto mouse_world_position = input_manager->mouse_world_position();

		CPTR_CAST<Navigator>(FindComponent(TAG{ NAVIGATOR, 0 }))->Route(position, mouse_world_position);
	}
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
