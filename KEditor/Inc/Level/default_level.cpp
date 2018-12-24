#include "stdafx.h"
#include "default_level.h"

#include "Actor/default_actor.h"
#include "Actor/text_actor.h"

#include "../MainFrm.h"

#include <Object/Actor/Monster/cow.h>
#include <Object/Actor/Monster/wendigo.h>
#include <Object/Actor/Monster/fallen_shaman.h>
#include <Object/Actor/Monster/andariel.h>
#include <Object/Actor/NPC/akara.h>

extern K::Vector2 g_mouse_LT;
extern K::Vector2 g_mouse_RB;

void K::DefaultLevel::Initialize()
{
	try
	{
		auto tile_layer = CreateLayer({ "TileLayer", 0 });
		auto layer = CreateLayer({ "DefaultLayer", 1 });
		auto text_layer = CreateLayer({ "TextLayer", 2 });

		auto const& object_manager = ObjectManager::singleton();

		auto tile_map = object_manager->CreateActor<TileMapActor>(TAG{ TILE_MAP, 0 });
		tile_layer->AddActor(tile_map);

		auto text = object_manager->CreateActor<TextActor>(TAG{ "TextActor", 0 });
		text->set_ui_flag(true);
		text_layer->AddActor(text);

		auto text1 = object_manager->CreateActor<TextActor>(TAG{ "TextActor", 1 });
		text1->set_ui_flag(true);
		APTR_CAST<TextActor>(text1)->set_color(DirectX::Colors::Green.v);
		CPTR_CAST<Transform>(text1->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ 550.f, static_cast<float>(RESOLUTION::HEIGHT), 0.f });
		text_layer->AddActor(text1);

		auto text2 = object_manager->CreateActor<TextActor>(TAG{ "TextActor", 2 });
		text2->set_ui_flag(true);
		APTR_CAST<TextActor>(text2)->set_color(DirectX::Colors::Red.v);
		CPTR_CAST<Transform>(text2->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ 1100.f, static_cast<float>(RESOLUTION::HEIGHT), 0.f });
		text_layer->AddActor(text2);
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
	auto const& camera_transform = CPTR_CAST<Transform>(WorldManager::singleton()->FindCamera(TAG{ DEFAULT_CAMERA, 0 })->FindComponent(TAG{ TRANSFORM, 0 }));

	if (input_manager->KeyPressed("W"))
		camera_transform->set_local_translation(camera_transform->local_translation() + Vector3::UnitY * 500.f * _time);

	if (input_manager->KeyPressed("S"))
		camera_transform->set_local_translation(camera_transform->local_translation() - Vector3::UnitY * 500.f * _time);

	if (input_manager->KeyPressed("A"))
		camera_transform->set_local_translation(camera_transform->local_translation() - Vector3::UnitX * 500.f * _time);

	if (input_manager->KeyPressed("D"))
		camera_transform->set_local_translation(camera_transform->local_translation() + Vector3::UnitX * 500.f * _time);

	camera_transform->Update(_time);

	if (input_manager->KeyDown("F1"))
		state_ = EDITOR_STATE::NONE;

	if (input_manager->KeyDown("F2"))
		state_ = EDITOR_STATE::TILE;

	if (input_manager->KeyDown("F3"))
		state_ = EDITOR_STATE::OPTION;

	if (input_manager->KeyDown("F4"))
		state_ = EDITOR_STATE::ACTOR;

	auto const& tile_map = APTR_CAST<TileMapActor>(WorldManager::singleton()->FindActor(TAG{ TILE_MAP, 0 }));

	CMainFrame* main_frame = static_cast<CMainFrame*>(AfxGetMainWnd());
	auto const& form_view = static_cast<CKEditorFormView*>(static_cast<CMainFrame*>(main_frame)->splitter().GetPane(0, 0));
	auto const& view = static_cast<CMainFrame*>(main_frame)->splitter().GetPane(0, 1);

	if (input_manager->KeyDown("F5"))
		form_view->option_combo_box().SetCurSel(0);

	if (input_manager->KeyDown("F6"))
		form_view->option_combo_box().SetCurSel(1);

	POINT mouse_position{};
	GetCursorPos(&mouse_position);
	ScreenToClient(view->m_hWnd, &mouse_position);
	mouse_position.y = static_cast<int>(RESOLUTION::HEIGHT) - mouse_position.y;

	auto const& camera = WorldManager::singleton()->FindCamera(TAG{ DEFAULT_CAMERA, 0 });
	auto const& camera_position = CPTR_CAST<Transform>(camera->FindComponent(TAG{ TRANSFORM, 0 }))->world().Translation();
	mouse_position.x += static_cast<LONG>(camera_position.x - static_cast<float>(RESOLUTION::WIDTH) * 0.5f);
	mouse_position.y += static_cast<LONG>(camera_position.y - static_cast<float>(RESOLUTION::HEIGHT) * 0.5f);

	auto tile_idx = tile_map->GetTileIndex(Vector3{ static_cast<float>(mouse_position.x), static_cast<float>(mouse_position.y) - 300.f, 0.f });

	if (input_manager->KeyPressed("LButton") && (state_ == EDITOR_STATE::OPTION))
		tile_map->SetTileOption(tile_idx, form_view->GetTileOption());

	if (input_manager->KeyPressed("LButton") && (state_ == EDITOR_STATE::TILE))
	{
		tile_map->SetTileUV(tile_idx, g_mouse_LT, g_mouse_RB);
	}

	if (input_manager->KeyPressed("RButton") && (state_ == EDITOR_STATE::TILE))
	{
		tile_map->SetTileUV(tile_idx, Vector2::Zero, Vector2::Zero);
		tile_map->SetTileOption(tile_idx, K::TILE_OPTION::BLOCKED);

		if (tile_idx.first < 0 || tile_idx.first >= form_view->count_x() || tile_idx.second < 0 || tile_idx.second >= form_view->count_y())
			return;

		APTR actor = WorldManager::singleton()->FindActor(tile_map->GetActorTag(tile_idx));

		if (actor)
		{
			tile_map->SetActorTag(tile_idx, TAG{});
			
			auto const& layer = FindLayer(TAG{ "DefaultLayer", 1 });
			layer->RemoveActor(actor);

			auto& actor_list = form_view->actor_list();
			actor_list.remove(actor);
		}
	}

	if (input_manager->KeyPressed("LButton") && (state_ == EDITOR_STATE::ACTOR))
	{
		if (mouse_position.x - (static_cast<LONG>(camera_position.x - static_cast<float>(RESOLUTION::WIDTH) * 0.5f)) < 10)
			return;

		if (mouse_position.x - (static_cast<LONG>(camera_position.x - static_cast<float>(RESOLUTION::WIDTH) * 0.5f)) > 1190)
			return;

		if (mouse_position.y - (static_cast<LONG>(camera_position.y - static_cast<float>(RESOLUTION::HEIGHT) * 0.5f)) < 310)
			return;

		if (mouse_position.y - (static_cast<LONG>(camera_position.y - static_cast<float>(RESOLUTION::HEIGHT) * 0.5f)) > 890)
			return;

		if (tile_idx.first < 0 || tile_idx.first >= form_view->count_x() || tile_idx.second < 0 || tile_idx.second >= form_view->count_y())
			return;

		if (false == tile_map->GetActorTag(tile_idx).first.empty())
			return;

		if (K::TILE_OPTION::BLOCKED == tile_map->GetTileOption(tile_idx))
			return;

		// 몬스터 생성
		std::string actor_type = form_view->GetActorType();

		Vector3 translation = tile_map->GetTilePosition(tile_idx);

		auto const& object_manager = ObjectManager::singleton();

		APTR actor{};
		static uint32_t counter{};

		if (actor_type == "1. Cow")
			actor = object_manager->CreateActor<Cow>(TAG{ "Cow", counter++ });
		else if (actor_type == "2. Wendigo")
			actor = object_manager->CreateActor<Wendigo>(TAG{ "Wendigo", counter++ });
		else if (actor_type == "3. Fallen Shaman")
			actor = object_manager->CreateActor<FallenShaman>(TAG{ "FallenShaman", counter++ });
		else if (actor_type == "4. Andariel")
			actor = object_manager->CreateActor<Andariel>(TAG{ "Andariel", counter++ });
		else if (actor_type == "5. Akara")
			actor = object_manager->CreateActor<Akara>(TAG{ "Akara", counter++ });

		auto const& actor_transform = CPTR_CAST<Transform>(actor->FindComponent(TAG{ TRANSFORM, 0 }));

		actor_transform->set_local_translation(translation);

		tile_map->SetActorTag(tile_idx, actor->tag());

		auto const& layer = FindLayer(TAG{ "DefaultLayer", 1 });
		layer->AddActor(actor);

		auto& actor_list = form_view->actor_list();
		actor_list.push_back(actor);
	}

	if (input_manager->KeyPressed("RButton") && (state_ == EDITOR_STATE::ACTOR))
	{
		if (mouse_position.y - (static_cast<LONG>(camera_position.y - static_cast<float>(RESOLUTION::HEIGHT) * 0.5f)) < 0)
			return;

		if (tile_idx.first < 0 || tile_idx.first >= form_view->count_x() || tile_idx.second < 0 || tile_idx.second >= form_view->count_y())
			return;

		APTR actor = WorldManager::singleton()->FindActor(tile_map->GetActorTag(tile_idx));

		if (actor)
		{
			tile_map->SetActorTag(tile_idx, TAG{});

			auto const& layer = FindLayer(TAG{ "DefaultLayer", 1 });
			layer->RemoveActor(actor);

			auto& actor_list = form_view->actor_list();
			actor_list.remove(actor);
		}
	}

	auto const& text = WorldManager::singleton()->FindActor(TAG{ "TextActor", 0 });

	std::wstring idx_text = L"Tile Index: " + std::to_wstring(tile_idx.first) + L", " + std::to_wstring(tile_idx.second);

	std::static_pointer_cast<TextActor>(text)->set_text(idx_text);

	std::wstring mode_text{};

	switch (state_)
	{
	case EDITOR_STATE::NONE:
		mode_text = L"None";
		break;

	case EDITOR_STATE::TILE:
		mode_text = L"Tile";
		break;

	case EDITOR_STATE::OPTION:
		mode_text = L"Option";
		break;

	case EDITOR_STATE::ACTOR:
		mode_text = L"Actor";
		break;
	}

	auto const& text1 = WorldManager::singleton()->FindActor(TAG{ "TextActor", 1 });
	std::static_pointer_cast<TextActor>(text1)->set_text(L"Mode: " + mode_text);

	auto const& text2 = WorldManager::singleton()->FindActor(TAG{ "TextActor", 2 });
	std::static_pointer_cast<TextActor>(text2)->set_text(L"FPS: " + std::to_wstring(TimeManager::singleton()->FPS()));
}

void K::DefaultLevel::_Update(float _time)
{
}

void K::DefaultLevel::_Render(float _time)
{
	auto const& context = DeviceManager::singleton()->context();

	D3D11_VIEWPORT viewport{};
	viewport.TopLeftY = -300.f;
	viewport.Width = 1600.f;
	viewport.Height = 900.f;
	viewport.MaxDepth = 1.f;

	context->RSSetViewports(1, &viewport);
}
