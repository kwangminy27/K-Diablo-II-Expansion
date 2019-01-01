#include "KEngine.h"
#include "fallen_shaman_fireball_explosion.h"

#include "Resource/resource_manager.h"
#include "Rendering/rendering_manager.h"
#include "Object/object_manager.h"
#include "Object/Actor/Player/player_actor.h"
#include "Object/Actor/Monster/monster_actor.h"
#include "Object/Component/transform.h"
#include "Object/Component/material.h"
#include "Object/Component/renderer.h"
#include "Object/Component/animation_2d.h"
#include "Object/Component/Collider/collider_aabb.h"

void K::FallenShamanFireballExplosion::Initialize()
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
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("fallen_shaman_fireball_explosion");
		AddComponent(animation_2d);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "FallenShamanFireballExplosion::Initialize" << std::endl;
	}
}

K::APTR K::FallenShamanFireballExplosion::Clone() const
{
	return APTR{ new FallenShamanFireballExplosion{ *this }, [](FallenShamanFireballExplosion* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::FallenShamanFireballExplosion::Serialize(InputMemoryStream& _imstream)
{
}

void K::FallenShamanFireballExplosion::Serialize(OutputMemoryStream& _omstream)
{
}

K::FallenShamanFireballExplosion::FallenShamanFireballExplosion(FallenShamanFireballExplosion const& _other) : MissileActor(_other)
{
}

K::FallenShamanFireballExplosion::FallenShamanFireballExplosion(FallenShamanFireballExplosion&& _other) noexcept : MissileActor(std::move(_other))
{
}

void K::FallenShamanFireballExplosion::_Finalize()
{
}
