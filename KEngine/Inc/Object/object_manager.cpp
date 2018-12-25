#include "KEngine.h"
#include "object_manager.h"

void K::ObjectManager::Initialize()
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
		std::cout << "ObjectManager::Initialize" << std::endl;
	}
}

uint32_t K::ObjectManager::counter()
{
	return counter_++;
}

void K::ObjectManager::_Finalize()
{
}
