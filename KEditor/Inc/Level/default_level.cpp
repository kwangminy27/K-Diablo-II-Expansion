#include "stdafx.h"
#include "default_level.h"

#include "Actor/default_actor.h"
#include "Actor/text_actor.h"

#include "../MainFrm.h"

extern K::Vector2 g_mouse_LT;
extern K::Vector2 g_mouse_RB;

void K::DefaultLevel::Initialize()
{
	try
	{
		auto layer = CreateLayer({ "DefaultLayer", 0 });

		auto const& object_manager = ObjectManager::singleton();

		auto tile_map = object_manager->CreateActor<TileMapActor>(TAG{ TILE_MAP, 0 });
		layer->AddActor(tile_map);

		auto text = object_manager->CreateActor<TextActor>(TAG{ "TextActor", 0 });
		text->set_ui_flag(true);
		layer->AddActor(text);

		auto text1 = object_manager->CreateActor<TextActor>(TAG{ "TextActor", 1 });
		text1->set_ui_flag(true);
		APTR_CAST<TextActor>(text1)->set_color(DirectX::Colors::Green.v);
		CPTR_CAST<Transform>(text1->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ 550.f, static_cast<float>(RESOLUTION::HEIGHT), 0.f });
		layer->AddActor(text1);

		auto text2 = object_manager->CreateActor<TextActor>(TAG{ "TextActor", 2 });
		text2->set_ui_flag(true);
		APTR_CAST<TextActor>(text2)->set_color(DirectX::Colors::Red.v);
		CPTR_CAST<Transform>(text2->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ 1100.f, static_cast<float>(RESOLUTION::HEIGHT), 0.f });
		layer->AddActor(text2);
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
		tile_map->SetTileUV(tile_idx, g_mouse_LT, g_mouse_RB);

	auto const& text = WorldManager::singleton()->FindActor(TAG{ "TextActor", 0 });
	std::static_pointer_cast<TextActor>(text)->set_text(L"Tile Index: " + std::to_wstring(tile_idx.first) + L", " + std::to_wstring(tile_idx.second));

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
