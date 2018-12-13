#include "KClient.h"
#include "Level/default_level.h"

int WINAPI wWinMain(HINSTANCE _instance, HINSTANCE _prev_instance, PWSTR _cmd_line, int _cmd_show)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	auto const& core = K::Core::singleton();
	auto const& world_manager = K::WorldManager::singleton();

	core->Initialize(L"K Game Engine", L"K Game Engine", _instance);

	world_manager->CreateLevel<K::DefaultLevel>({ "DefaultLevel", 0 });

	core->Run();

	K::Core::singleton().reset();

	return 0;
}
