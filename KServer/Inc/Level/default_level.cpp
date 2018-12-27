#include "KServer.h"
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
		auto ui_layer = CreateLayer({ "UILayer", 2 });

		auto const& object_manager = ObjectManager::singleton();
		auto const& registry_manager = RegistryManager::singleton();
		auto const& replication_manager = ReplicationManager::singleton();

		registry_manager->AddActorGenerator({ "Cow" }, [&object_manager](TAG const& _tag) -> APTR {
			return object_manager->CreateActor<Cow>(_tag);
		});

		// Map & Monster 로드
		auto const& path_manager = PathManager::singleton();

		std::wstring file_name = path_manager->FindPath(DATA_PATH).wstring() + L"test.lv";

		std::fstream file{ file_name, std::ios::in | std::ios::binary | std::ios::ate };

		auto tile_map = object_manager->CreateActor<TileMapActor>(TAG{ TILE_MAP, object_manager->counter() });

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

			replication_manager->AddActor(actor);
			layer->AddActor(actor);
		}
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
	auto const& replication_manager = ReplicationManager::singleton();
	auto const& connection_manager = ConnectionManager::singleton();

	if (input_manager->KeyDown("Space"))
	{
		OutputMemoryStream omstream;
		replication_manager->SendWorld(omstream); // 여기까지 수행되면 데이터가 omstream에 들어있게 됨
	}
}

void K::DefaultLevel::_Update(float _time)
{

}

void K::DefaultLevel::_Collision(float _time)
{

}

void K::DefaultLevel::__Render(float _time)
{
}
