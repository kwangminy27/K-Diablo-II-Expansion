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

#include "Actor/text_actor.h"

void K::DefaultLevel::Initialize()
{
	try
	{
		auto tile_layer = CreateLayer({ "TileLayer", 0 });
		auto layer = CreateLayer({ "DefaultLayer", 1 });
		auto ui_layer = CreateLayer({ "UILayer", 2 });
		auto text_layer = CreateLayer({ "TextLayer", 3 });
		
		auto const& object_manager = ObjectManager::singleton();
		auto const& registry_manager = RegistryManager::singleton();

		registry_manager->AddActorGenerator({ "Cow" }, [&object_manager](TAG const& _tag) -> APTR {
			return object_manager->CreateActor<Cow>(_tag);
		});

		registry_manager->AddActorGenerator({ "Wendigo" }, [&object_manager](TAG const& _tag) -> APTR {
			return object_manager->CreateActor<Wendigo>(_tag);
		});

		registry_manager->AddActorGenerator({ "FallenShaman" }, [&object_manager](TAG const& _tag) -> APTR {
			return object_manager->CreateActor<FallenShaman>(_tag);
		});

		registry_manager->AddActorGenerator({ "Andariel" }, [&object_manager](TAG const& _tag) -> APTR {
			return object_manager->CreateActor<Andariel>(_tag);
		});

		registry_manager->AddActorGenerator({ "Akara" }, [&object_manager](TAG const& _tag) -> APTR {
			return object_manager->CreateActor<Akara>(_tag);
		});

		// Map & Monster ·Îµå
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

		//size_t actor_size{};
		//imstream.Serialize(actor_size);

		//for (size_t i = 0; i < actor_size; ++i)
		//{
		//	K::TAG tag{};

		//	imstream.Serialize(tag.first);
		//	imstream.Serialize(tag.second);

		//	K::APTR actor{};

		//	if (tag.first == "Cow")
		//		actor = object_manager->CreateActor<K::Cow>(tag);
		//	else if (tag.first == "Wendigo")
		//		actor = object_manager->CreateActor<K::Wendigo>(tag);
		//	else if (tag.first == "FallenShaman")
		//		actor = object_manager->CreateActor<K::FallenShaman>(tag);
		//	else if (tag.first == "Andariel")
		//		actor = object_manager->CreateActor<K::Andariel>(tag);
		//	else if (tag.first == "Akara")
		//		actor = object_manager->CreateActor<K::Akara>(tag);

		//	actor->Serialize(imstream);
		//	layer->AddActor(actor);
		//}

		//auto cow = object_manager->CreateActor<Cow>(TAG{ "Cow", object_manager->counter() });
		//layer->AddActor(cow);

		//auto wendigo = object_manager->CreateActor<Wendigo>(TAG{ "Wendigo", object_manager->counter() });
		//CPTR_CAST<Transform>(wendigo->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ -100.f, 150.f, 0.f });
		//layer->AddActor(wendigo);

		//auto fallen_shaman = object_manager->CreateActor<FallenShaman>(TAG{ "FallenShaman", object_manager->counter() });
		//CPTR_CAST<Transform>(fallen_shaman->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ 100.f, 150.f, 0.f });
		//layer->AddActor(fallen_shaman);

		//auto andariel = object_manager->CreateActor<Andariel>(TAG{ "Andariel", object_manager->counter() });
		//CPTR_CAST<Transform>(andariel->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ 0.f, 2000.f, 0.f });
		//layer->AddActor(andariel);

		//auto akara = object_manager->CreateActor<Akara>(TAG{ "Akara", object_manager->counter() });
		//CPTR_CAST<Transform>(akara->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ -200.f, 300.f, 0.f });
		//layer->AddActor(akara);

		player_tag_ = TAG{ "Sorceress", object_manager->counter() };

		auto sorceress = object_manager->CreateActor<Sorceress>(player_tag_);
		APTR_CAST<PlayerActor>(sorceress)->set_focus_flag(true);
		CPTR_CAST<Transform>(sorceress->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ 0.f, 0.f, 0.f });
		layer->AddActor(sorceress);

		//auto amazon = object_manager->CreateActor<Amazon>(TAG{ "Amazon", object_manager->counter() });
		//CPTR_CAST<Transform>(amazon->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ 0.f, -150.f, 0.f });
		//layer->AddActor(amazon);

		auto text = object_manager->CreateActor<TextActor>(TAG{ "TextActor", 0 });
		auto const& text_transform = CPTR_CAST<Transform>(text->FindComponent(TAG{ TRANSFORM, 0 }));
		text_transform->set_local_translation(Vector3{ 0.f, 25.f, 0.f });
		text->set_ui_flag(true);
		text_layer->AddActor(text);

		auto text_1 = object_manager->CreateActor<TextActor>(TAG{ "TextActor", 1 });
		auto const& text_1_transform = CPTR_CAST<Transform>(text_1->FindComponent(TAG{ TRANSFORM, 0 }));
		text_1_transform->set_local_translation(Vector3{ 0.f, 50.f, 0.f });
		text_1->set_ui_flag(true);
		text_layer->AddActor(text_1);

		auto text_2 = object_manager->CreateActor<TextActor>(TAG{ "TextActor", 2 });
		auto const& text_2_transform = CPTR_CAST<Transform>(text_2->FindComponent(TAG{ TRANSFORM, 0 }));
		text_2_transform->set_local_translation(Vector3{ 0.f, 75.f, 0.f });
		text_2->set_ui_flag(true);
		text_layer->AddActor(text_2);

		auto text_3 = object_manager->CreateActor<TextActor>(TAG{ "TextActor", 3 });
		auto const& text_3_transform = CPTR_CAST<Transform>(text_3->FindComponent(TAG{ TRANSFORM, 0 }));
		text_3_transform->set_local_translation(Vector3{ 0.f, 100.f, 0.f });
		text_3->set_ui_flag(true);
		text_layer->AddActor(text_3);

		auto text_4 = object_manager->CreateActor<TextActor>(TAG{ "TextActor", 4 });
		auto const& text_4_transform = CPTR_CAST<Transform>(text_4->FindComponent(TAG{ TRANSFORM, 0 }));
		text_4_transform->set_local_translation(Vector3{ 0.f, 125.f, 0.f });
		text_4->set_ui_flag(true);
		text_layer->AddActor(text_4);

		auto text_5 = object_manager->CreateActor<TextActor>(TAG{ "TextActor", 5 });
		auto const& text_5_transform = CPTR_CAST<Transform>(text_5->FindComponent(TAG{ TRANSFORM, 0 }));
		text_5_transform->set_local_translation(Vector3{ 0.f, 150.f, 0.f });
		text_5->set_ui_flag(true);
		text_layer->AddActor(text_5);

		auto const& audio_manager = AudioManager::singleton();

		auto const& BGM = audio_manager->FindSoundEffect("BGM");
		auto BGM_instance = BGM->CreateInstance();
		BGM_instance->Play(true);
		audio_manager->AddSoundEffectInstance("BGM", std::move(BGM_instance));

		auto const& rain = audio_manager->FindSoundEffect("rain");
		auto rain_instance = rain->CreateInstance();
		rain_instance->Play(true);
		audio_manager->AddSoundEffectInstance("rain", std::move(rain_instance));
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

		auto const& sorceress = WorldManager::singleton()->FindActor(player_tag_);
		APTR_CAST<Sorceress>(sorceress)->set_focus_flag((counter++) % 2);
	}

	auto const& camera_transform = CPTR_CAST<Transform>(WorldManager::singleton()->FindCamera(TAG{ DEFAULT_CAMERA, 0 })->FindComponent(TAG{ TRANSFORM, 0 }));

	if (input_manager->KeyPressed("Up"))
		camera_transform->set_local_translation(camera_transform->local_translation() + Vector3::UnitY * 1000.f * _time);
	if (input_manager->KeyPressed("Left"))
		camera_transform->set_local_translation(camera_transform->local_translation() - Vector3::UnitX * 1000.f * _time);
	if (input_manager->KeyPressed("Down"))
		camera_transform->set_local_translation(camera_transform->local_translation() - Vector3::UnitY * 1000.f * _time);
	if (input_manager->KeyPressed("Right"))
		camera_transform->set_local_translation(camera_transform->local_translation() + Vector3::UnitX * 1000.f * _time);

	camera_transform->Update(_time);
}

void K::DefaultLevel::_Render(float _time)
{
	auto const& text = WorldManager::singleton()->FindActor(TAG{ "TextActor", 0 });
	
	auto chat_message = Core::singleton()->chat_message();

	std::wstring message{ chat_message.begin(), chat_message.end() };

	APTR_CAST<TextActor>(text)->set_text(message);
}

//void K::DefaultLevel::__Collision(float _time)
//{
//};
