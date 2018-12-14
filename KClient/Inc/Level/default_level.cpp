#include "KClient.h"
#include "default_level.h"

#include "Actor/default_actor.h"

void K::DefaultLevel::Initialize()
{
	try
	{
		auto layer = CreateLayer({ "DefaultLayer", 0 });

		auto const& object_manager = ObjectManager::singleton();
		auto const& registry_manager = RegistryManager::singleton();

		registry_manager->AddActorGenerator({ "DefaultActor" }, [&object_manager](TAG const& _tag) -> APTR {
			return object_manager->CreateActor<DefaultActor>(_tag);
		});

		auto tile_map = object_manager->CreateActor<TileMapActor>(TAG{ TILE_MAP, 0 });
		std::static_pointer_cast<TileMapActor>(tile_map)->CreateMap(TILE_TYPE::ISOMETRIC, 100, 100, Vector2{ 16.f, 8.f });
		layer->AddActor(tile_map);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "DefaultLevel::Initialize" << std::endl;
	}
}

K::DefaultLevel::DefaultLevel(DefaultLevel&& _other) noexcept : Level(std::move(_other))
{
}

void K::DefaultLevel::_Finalize()
{
}
