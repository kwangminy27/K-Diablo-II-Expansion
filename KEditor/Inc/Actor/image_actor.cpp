#include "stdafx.h"
#include "image_actor.h"

#include "Component/default_component.h"

void K::ImageActor::Initialize()
{
	try
	{
		auto const& object_manager = ObjectManager::singleton();

		auto transform = object_manager->CreateComponent<Transform>(TAG{ TRANSFORM, 0 });
		CPTR_CAST<Transform>(transform)->set_local_scaling(Vector3{ 160.f, 80.f, 1.f });
		CPTR_CAST<Transform>(transform)->set_local_translation(Vector3::Zero);
		AddComponent(transform);

		auto material = object_manager->CreateComponent<Material>(TAG{ MATERIAL, 0 });

		CPTR_CAST<Material>(material)->SetTexture("catacomb floor", 0, 0, 0);
		CPTR_CAST<Material>(material)->SetSampler(LINEAR_SAMPLER, 0, 0, 0);

		MaterialConstantBuffer Material_CB{};
		Material_CB.diffuse = DirectX::Colors::White.v;
		CPTR_CAST<Material>(material)->SetMaterialConstantBuffer(Material_CB, 0, 0);

		AddComponent(material);

		auto const& rendering_manager = RenderingManager::singleton();

		auto renderer = object_manager->CreateComponent<Renderer>(TAG{ RENDERER, 0 });
		CPTR_CAST<Renderer>(renderer)->set_mesh(ResourceManager::singleton()->FindMesh(TEX_RECT));
		CPTR_CAST<Renderer>(renderer)->set_shader(rendering_manager->FindShader(BASIC_ANIMATION_2D_SHADER));
		CPTR_CAST<Renderer>(renderer)->set_render_state(rendering_manager->FindRenderState(DEPTH_DISABLE));
		CPTR_CAST<Renderer>(renderer)->set_render_state(rendering_manager->FindRenderState(ALPHA_BLEND));
		AddComponent(renderer);

		auto collider = object_manager->CreateComponent<ColliderAABB>(TAG{ COLLIDER, 0 });
		CPTR_CAST<ColliderAABB>(collider)->set_group_tag(UI);
		CPTR_CAST<ColliderAABB>(collider)->set_relative_info(AABB{ Vector2::Zero, Vector2{ 80.f, 40.f } });
		AddComponent(collider);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "ImageActor::Initialize" << std::endl;
	}
}

K::APTR K::ImageActor::Clone() const
{
	return APTR{ new ImageActor{ *this }, [](ImageActor* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::ImageActor::Serialize(InputMemoryStream& _imstream)
{
}

void K::ImageActor::Serialize(OutputMemoryStream& _omstream)
{
}

void K::ImageActor::set_LT(Vector2 const& _LT)
{
	LT_ = _LT;
}

void K::ImageActor::set_RB(Vector2 const& _RB)
{
	RB_ = _RB;
}

K::ImageActor::ImageActor(ImageActor const& _other) : ActorClient(_other)
{
	LT_ = _other.LT_;
	RB_ = _other.RB_;
}

K::ImageActor::ImageActor(ImageActor&& _other) noexcept : ActorClient(std::move(_other))
{
	LT_ = std::move(_other.LT_);
	RB_ = std::move(_other.RB_);
}

void K::ImageActor::_Finalize()
{
}

void K::ImageActor::_Render(float _time)
{
	Animation2DConstantBuffer animation_2d_CB{};
	animation_2d_CB.LT = LT_;
	animation_2d_CB.RB = RB_;

	RenderingManager::singleton()->UpdateConstantBuffer(ANIMATION_2D, &animation_2d_CB);
}
