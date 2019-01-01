#include "KEngine.h"
#include "frozen_armor.h"

#include "Resource/resource_manager.h"
#include "Rendering/rendering_manager.h"
#include "Object/object_manager.h"
#include "Object/Actor/Player/sorceress.h"
#include "Object/Component/transform.h"
#include "Object/Component/material.h"
#include "Object/Component/renderer.h"
#include "Object/Component/animation_2d.h"
#include "Object/Component/Collider/collider_aabb.h"

void K::FrozenArmor::Initialize()
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
		CPTR_CAST<Animation2D>(animation_2d)->AddClip("frozen_armor");
		CPTR_CAST<Animation2D>(animation_2d)->SetCurrentClip("frozen_armor", -1);
		AddComponent(animation_2d);

		life_time_ = 10.f;
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "FrozenArmor::Initialize" << std::endl;
	}
}

K::APTR K::FrozenArmor::Clone() const
{
	return APTR{ new FrozenArmor{ *this }, [](FrozenArmor* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::FrozenArmor::Serialize(InputMemoryStream& _imstream)
{
}

void K::FrozenArmor::Serialize(OutputMemoryStream& _omstream)
{
}

void K::FrozenArmor::set_target(std::shared_ptr<Actor> const& _target)
{
	target_ = _target;
}

K::FrozenArmor::FrozenArmor(FrozenArmor const& _other) : MissileActor(_other)
{
}

K::FrozenArmor::FrozenArmor(FrozenArmor&& _other) noexcept : MissileActor(std::move(_other))
{
}

void K::FrozenArmor::_Finalize()
{
}

void K::FrozenArmor::_Update(float _time)
{
	life_time_ -= _time;

	auto target = target_.lock();

	auto const& transform = CPTR_CAST<Transform>(FindComponent(TAG{ TRANSFORM, 0 }));
	auto const& target_transform = CPTR_CAST<Transform>(target->FindComponent(TAG{ TRANSFORM, 0 }));

	transform->set_local_translation(target_transform->world().Translation());

	if (life_time_ < 0.f)
	{
		APTR_CAST<Sorceress>(target)->set_frozen_armor_flag(false);
		set_tag_state(TAG_STATE::DEAD);
	}
}
