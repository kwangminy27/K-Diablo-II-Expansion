#include "KClient.h"
#include "default_level.h"

#include "Actor/default_actor.h"
#include <Object/Actor/Monster/cow.h>
#include <Object/Actor/Monster/wendigo.h>
#include <Object/Actor/Monster/fallen_shaman.h>
#include <Object/Actor/Monster/andariel.h>
#include <Object/Actor/NPC/akara.h>
#include <Object/Actor/Player/sorceress.h>
#include <Object/Actor/Player/amazon.h>

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
		std::static_pointer_cast<TileMapActor>(tile_map)->CreateMap(TILE_TYPE::ISOMETRIC, 10, 10, Vector2{ 160.f, 80.f });
		layer->AddActor(tile_map);

		auto cow = object_manager->CreateActor<Cow>(TAG{ "Cow", 0 });
		layer->AddActor(cow);

		auto wendigo = object_manager->CreateActor<Wendigo>(TAG{ "Wendigo", 0 });
		CPTR_CAST<Transform>(wendigo->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ -100.f, 150.f, 0.f });
		layer->AddActor(wendigo);

		auto fallen_shaman = object_manager->CreateActor<FallenShaman>(TAG{ "FallenShaman", 0 });
		CPTR_CAST<Transform>(fallen_shaman->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ 100.f, 150.f, 0.f });
		layer->AddActor(fallen_shaman);

		auto andariel = object_manager->CreateActor<Andariel>(TAG{ "Andariel", 0 });
		CPTR_CAST<Transform>(andariel->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ 0.f, 300.f, 0.f });
		layer->AddActor(andariel);

		auto akara = object_manager->CreateActor<Akara>(TAG{ "Akara", 0 });
		CPTR_CAST<Transform>(akara->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ -200.f, 300.f, 0.f });
		layer->AddActor(akara);

		auto sorceress = object_manager->CreateActor<Sorceress>(TAG{ "Sorceress", 0 });
		CPTR_CAST<Transform>(sorceress->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ 200.f, 300.f, 0.f });
		layer->AddActor(sorceress);

		auto amazon = object_manager->CreateActor<Amazon>(TAG{ "Amazon", 0 });
		CPTR_CAST<Transform>(amazon->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ 0.f, -150.f, 0.f });
		layer->AddActor(amazon);
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
