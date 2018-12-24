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
		auto tile_layer = CreateLayer({ "TileLayer", 0 });
		auto layer = CreateLayer({ "DefaultLayer", 1 });

		auto const& object_manager = ObjectManager::singleton();
		auto const& registry_manager = RegistryManager::singleton();

		registry_manager->AddActorGenerator({ "DefaultActor" }, [&object_manager](TAG const& _tag) -> APTR {
			return object_manager->CreateActor<DefaultActor>(_tag);
		});

		// Map & Monster ·Îµå
		auto const& path_manager = PathManager::singleton();

		std::wstring file_name = path_manager->FindPath(DATA_PATH).wstring() + L"test.lv";

		std::fstream file{ file_name, std::ios::in | std::ios::binary | std::ios::ate };

		auto tile_map = object_manager->CreateActor<TileMapActor>(TAG{ TILE_MAP, 0 });
		
		auto size = file.tellg();
		file.seekg(std::ios::beg);

		K::InputMemoryStream imstream{};
		imstream.Resize(size);
		file.read(reinterpret_cast<char*>(imstream.buffer()->data()), size);

		tile_map->Serialize(imstream);

		NavigationManager::singleton()->AddTileMap(APTR_CAST<TileMapActor>(tile_map));
		tile_layer->AddActor(tile_map);

		size_t actor_size{};
		imstream.Serialize(actor_size);

		for (size_t i = 0; i < actor_size; ++i)
		{
			K::TAG tag{};

			imstream.Serialize(tag.first);
			imstream.Serialize(tag.second);

			K::APTR actor{};

			if (tag.first == "Cow")
				actor = object_manager->CreateActor<K::Cow>(tag);
			else if (tag.first == "Wendigo")
				actor = object_manager->CreateActor<K::Wendigo>(tag);
			else if (tag.first == "FallenShaman")
				actor = object_manager->CreateActor<K::FallenShaman>(tag);
			else if (tag.first == "Andariel")
				actor = object_manager->CreateActor<K::Andariel>(tag);
			else if (tag.first == "Akara")
				actor = object_manager->CreateActor<K::Akara>(tag);

			actor->Serialize(imstream);
			layer->AddActor(actor);
		}
		//

		//auto cow = object_manager->CreateActor<Cow>(TAG{ "Cow", 0 });
		//layer->AddActor(cow);

		//auto wendigo = object_manager->CreateActor<Wendigo>(TAG{ "Wendigo", 0 });
		//CPTR_CAST<Transform>(wendigo->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ -100.f, 150.f, 0.f });
		//layer->AddActor(wendigo);

		//auto fallen_shaman = object_manager->CreateActor<FallenShaman>(TAG{ "FallenShaman", 0 });
		//CPTR_CAST<Transform>(fallen_shaman->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ 100.f, 150.f, 0.f });
		//layer->AddActor(fallen_shaman);

		//auto andariel = object_manager->CreateActor<Andariel>(TAG{ "Andariel", 0 });
		//CPTR_CAST<Transform>(andariel->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ 0.f, 300.f, 0.f });
		//layer->AddActor(andariel);

		//auto akara = object_manager->CreateActor<Akara>(TAG{ "Akara", 0 });
		//CPTR_CAST<Transform>(akara->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ -200.f, 300.f, 0.f });
		//layer->AddActor(akara);

		auto sorceress = object_manager->CreateActor<Sorceress>(TAG{ "Sorceress", 0 });
		APTR_CAST<PlayerActor>(sorceress)->set_focus_flag(true);
		CPTR_CAST<Transform>(sorceress->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ 0.f, 0.f, 0.f });
		layer->AddActor(sorceress);

		//auto amazon = object_manager->CreateActor<Amazon>(TAG{ "Amazon", 0 });
		//CPTR_CAST<Transform>(amazon->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ 0.f, -150.f, 0.f });
		//layer->AddActor(amazon);
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

void K::DefaultLevel::_Input(float _time)
{
	auto const& input_manager = InputManager::singleton();

	if(input_manager->KeyDown("F9"))
	{
		static int counter{};

		auto const& sorceress = WorldManager::singleton()->FindActor(TAG{ "Sorceress", 0 });
		APTR_CAST<Sorceress>(sorceress)->set_focus_flag((counter++) % 2);
	}
}