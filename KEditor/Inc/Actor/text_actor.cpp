#include "stdafx.h"
#include "text_actor.h"

void K::TextActor::Initialize()
{
	try
	{
		auto const& object_manager = ObjectManager::singleton();

		auto transform = object_manager->CreateComponent<Transform>({ TRANSFORM, 0 });
		CPTR_CAST<Transform>(transform)->set_local_translation(Vector3{ 0.f, static_cast<float>(RESOLUTION::HEIGHT), 0.f });
		AddComponent(transform);

		auto text = object_manager->CreateComponent<Text>({ TEXT, 0 });
		CPTR_CAST<Text>(text)->set_color_key(DirectX::Colors::White.v);
		CPTR_CAST<Text>(text)->set_format_tag("D2Coding");
		CPTR_CAST<Text>(text)->set_text(L"");
		CPTR_CAST<Text>(text)->set_text_area(D2D1::RectF(0.f, 0.f, 1600.f, 20.f));
		AddComponent(text);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "DefaultActor::Initialize" << std::endl;
	}
}

K::APTR K::TextActor::Clone() const
{
	return APTR{ new TextActor{ *this }, [](TextActor* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::TextActor::Serialize(InputMemoryStream& _imstream)
{
}

void K::TextActor::Serialize(OutputMemoryStream& _omstream)
{
}

void K::TextActor::Move(Vector3 const& _v)
{
	auto const& transform = CPTR_CAST<Transform>(FindComponent(TAG{ TRANSFORM, 0 }));

	transform->set_local_translation(transform->local_translation() + _v);
}

void K::TextActor::set_text(std::wstring const& _text)
{
	CPTR_CAST<Text>(FindComponent(TAG{ TEXT, 0 }))->set_text(_text);
}

void K::TextActor::set_color(Vector4 const& _color)
{
	CPTR_CAST<Text>(FindComponent(TAG{ TEXT, 0 }))->set_color_key(_color);
}

K::TextActor::TextActor(TextActor const& _other) : ActorClient(_other)
{
}

K::TextActor::TextActor(TextActor&& _other) noexcept : ActorClient(std::move(_other))
{
}

void K::TextActor::_Finalize()
{
}
