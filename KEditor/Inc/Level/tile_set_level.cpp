#include "stdafx.h"
#include "tile_set_level.h"

#include "Actor/text_actor.h"
#include "Actor/image_actor.h"

#include "../MainFrm.h"

K::Vector2 g_mouse_LT = K::Vector2{ (1.f / 4) * 2, (1.f / 17) * 14 };
K::Vector2 g_mouse_RB = K::Vector2{ (1.f / 4) * 3, (1.f / 17) * 15 };

void K::TileSetLevel::Initialize()
{
	try
	{
		auto layer = CreateLayer({ "DefaultLayer", 0 });

		auto const& object_manager = ObjectManager::singleton();

		for (auto i = 0; i < 17; ++i)
		{
			for (auto j = 0; j < 4; ++j)
			{
				auto image = object_manager->CreateActor<ImageActor>(TAG{ "ImageActor", 0 });
				image->set_ui_flag(true);
				APTR_CAST<ImageActor>(image)->set_LT(Vector2{ (1.f / 4) * j, (1.f / 17) * i });
				APTR_CAST<ImageActor>(image)->set_RB(Vector2{ (1.f / 4) * (j + 1), (1.f / 17) * (i + 1) });
				CPTR_CAST<Transform>(image->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_translation(Vector3{ -720.f + 161.f * i, 370.f - 81.f * j, 0.f });
				layer->AddActor(image);
			}
		}

		auto mouse = object_manager->CreateActor<ImageActor>(TAG{ "MouseActor", 0 });
		mouse->set_ui_flag(true);
		layer->AddActor(mouse);

		auto text = object_manager->CreateActor<TextActor>(TAG{ "TextActor", 0 });
		text->set_ui_flag(true);
		layer->AddActor(text);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "TileSetLevel::Initialize" << std::endl;
	}
}

K::TileSetLevel::TileSetLevel(TileSetLevel&& _other) noexcept : Level(std::move(_other))
{
}

void K::TileSetLevel::_Finalize()
{
}

void K::TileSetLevel::_Input(float _time)
{
	auto const& input_manager = InputManager::singleton();
	auto const& camera_transform = CPTR_CAST<Transform>(WorldManager::singleton()->FindCamera(TAG{ UI_CAMERA, 0 })->FindComponent(TAG{ TRANSFORM, 0 }));

	if (input_manager->KeyPressed("Up"))
	{
		camera_transform->set_local_translation(camera_transform->local_translation() + Vector3::UnitY * 1000.f * _time);

		auto camera_position = camera_transform->local_translation();

		if (camera_position.y > 0.f)
		{
			camera_position.y = 0.f;
			camera_transform->set_local_translation(camera_position);
		}
	}

	if (input_manager->KeyPressed("Down"))
	{
		camera_transform->set_local_translation(camera_transform->local_translation() - Vector3::UnitY * 1000.f * _time);

		auto camera_position = camera_transform->local_translation();

		if (camera_position.y < -23.f)
		{
			camera_position.y = -23.f;
			camera_transform->set_local_translation(camera_position);
		}
	}

	if (input_manager->KeyPressed("Left"))
	{
		camera_transform->set_local_translation(camera_transform->local_translation() - Vector3::UnitX * 1000.f * _time);

		auto camera_position = camera_transform->local_translation();

		if (camera_position.x < 0.f)
		{
			camera_position.x = 0.f;
			camera_transform->set_local_translation(camera_position);
		}
	}

	if (input_manager->KeyPressed("Right"))
	{
		camera_transform->set_local_translation(camera_transform->local_translation() + Vector3::UnitX * 1000.f * _time);

		auto camera_position = camera_transform->local_translation();

		if (camera_position.x > 1543.f)
		{
			camera_position.x = 1543.f;
			camera_transform->set_local_translation(camera_position);
		}
	}

	camera_transform->Update(_time);

	CMainFrame* main_frame = static_cast<CMainFrame*>(AfxGetMainWnd());
	auto const& form_view = static_cast<CKEditorFormView*>(static_cast<CMainFrame*>(main_frame)->splitter().GetPane(0, 0));
	auto const& view = static_cast<CMainFrame*>(main_frame)->splitter().GetPane(0, 1);

	POINT mouse_position{};
	GetCursorPos(&mouse_position);
	ScreenToClient(view->m_hWnd, &mouse_position);
	mouse_position.y = static_cast<int>(RESOLUTION::HEIGHT) - mouse_position.y;

	auto const& camera = WorldManager::singleton()->FindCamera(TAG{ UI_CAMERA, 0 });
	auto const& camera_position = CPTR_CAST<Transform>(camera->FindComponent(TAG{ TRANSFORM, 0 }))->world().Translation();
	mouse_position.x += static_cast<LONG>(camera_position.x);
	mouse_position.y += static_cast<LONG>(camera_position.y) + 20;

	int x_idx = static_cast<int>(mouse_position.x / 160.f);
	int y_idx = static_cast<int>(mouse_position.y / 80.f);

	if (input_manager->KeyDown("LButton") && x_idx >= 0 && x_idx <= 16 && y_idx >= 0 && y_idx <= 3)
	{
		g_mouse_LT = Vector2{ (1.f / 4) * (3 - y_idx), (1.f / 17) * x_idx };
		g_mouse_RB = Vector2{ (1.f / 4) * (3 - y_idx + 1), (1.f / 17) * (x_idx + 1) };
	}
}

void K::TileSetLevel::_Update(float _time)
{
}

void K::TileSetLevel::_Render(float _time)
{
	auto const& context = DeviceManager::singleton()->context();

	D3D11_VIEWPORT viewport{};
	viewport.TopLeftY = 600.f;
	viewport.Width = 1600.f;
	viewport.Height = 900.f;
	viewport.MaxDepth = 1.f;

	context->RSSetViewports(1, &viewport);
}
