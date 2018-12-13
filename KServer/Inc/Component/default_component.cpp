#include "KServer.h"
#include "default_component.h"

void K::DefaultComponent::Initialize()
{
	try
	{
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "DefaultComponent::Initialize" << std::endl;
	}
}

K::CPTR K::DefaultComponent::Clone() const
{
	return CPTR{ new DefaultComponent{ *this }, [](Component* _p) {
		static_cast<DefaultComponent*>(_p)->_Finalize();
		delete static_cast<DefaultComponent*>(_p);
	} };
}

void K::DefaultComponent::Serialize(InputMemoryStream& _imstream)
{
}

void K::DefaultComponent::Serialize(OutputMemoryStream& _omstream)
{
}

K::DefaultComponent::DefaultComponent(DefaultComponent const& _other) : Component(_other)
{
}

K::DefaultComponent::DefaultComponent(DefaultComponent&& _other) noexcept : Component(std::move(_other))
{
}

void K::DefaultComponent::_Finalize()
{
}
