#include "KEngine.h"
#include "input_manager.h"

#include "core.h"
#include "World/world_manager.h"
#include "Object/Actor/camera_actor.h"
#include "Object/Component/transform.h"

std::pair<std::string, K::KEY_DESC> K::InputManager::key_desc_dummy_{};

void K::InputManager::Initialize()
{
	try
	{
		_CreateKeyDesc(std::string{ "ESC" }, VK_ESCAPE);

		_CreateKeyDesc(std::string{ "W" }, 'W');
		_CreateKeyDesc(std::string{ "S" }, 'S');
		_CreateKeyDesc(std::string{ "A" }, 'A');
		_CreateKeyDesc(std::string{ "D" }, 'D');

		_CreateKeyDesc(std::string{ "O" }, 'O');
		_CreateKeyDesc(std::string{ "P" }, 'P');

		_CreateKeyDesc(std::string{ "R" }, 'R');

		_CreateKeyDesc(std::string{ "Up" }, VK_UP);
		_CreateKeyDesc(std::string{ "Down" }, VK_DOWN);
		_CreateKeyDesc(std::string{ "Left" }, VK_LEFT);
		_CreateKeyDesc(std::string{ "Right" }, VK_RIGHT);

		_CreateKeyDesc(std::string{ "LButton" }, VK_LBUTTON);
		_CreateKeyDesc(std::string{ "RButton" }, VK_RBUTTON);

		_CreateKeyDesc(std::string{ "F1" }, VK_F1 );
		_CreateKeyDesc(std::string{ "F2" }, VK_F2 );
		_CreateKeyDesc(std::string{ "F3" }, VK_F3 );
		_CreateKeyDesc(std::string{ "F4" }, VK_F4);
		_CreateKeyDesc(std::string{ "F5" }, VK_F5 );
		_CreateKeyDesc(std::string{ "F6" }, VK_F6 );
		_CreateKeyDesc(std::string{ "F7" }, VK_F7);
		_CreateKeyDesc(std::string{ "F8" }, VK_F8);
		_CreateKeyDesc(std::string{ "F9" }, VK_F9);
		_CreateKeyDesc(std::string{ "F10" }, VK_F10);
		_CreateKeyDesc(std::string{ "F11" }, VK_F11);
		_CreateKeyDesc(std::string{ "F12" }, VK_F12);

		_CreateKeyDesc(std::string{ "Space" }, VK_SPACE);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "InputManager::Initialize" << std::endl;
	}
}

void K::InputManager::Update()
{
	for (auto iter = key_desc_map_.begin(); iter != key_desc_map_.end(); ++iter)
	{
		auto& key_desc = iter->second;

		int count{};
		for (auto const& key : key_desc.key_vector)
		{
			if (GetAsyncKeyState(key) & 0x8000)
				++count;
		}

		if (count == key_desc.key_vector.size())
		{
			if (!key_desc.pressed)
			{
				key_desc.down = true;
				key_desc.pressed = true;
			}
			else
				key_desc.down = false;
		}
		else if (key_desc.pressed)
		{
			key_desc.down = false;
			key_desc.pressed = false;
			key_desc.up = true;
		}
		else if (key_desc.up)
			key_desc.up = false;
	}

	auto const& camera = WorldManager::singleton()->FindCamera(TAG{ DEFAULT_CAMERA, 0 });
	auto const& camera_position = CPTR_CAST<Transform>(camera->FindComponent(TAG{ TRANSFORM, 0 }))->world().Translation();
	auto resolution = camera->resolution();
	auto scale = camera->scale();

	POINT mouse_position{};
	GetCursorPos(&mouse_position);
	ScreenToClient(Core::singleton()->window(), &mouse_position);
	mouse_position.x = static_cast<LONG>(mouse_position.x * scale);
	mouse_position.y = static_cast<LONG>(mouse_position.y * scale);

	mouse_position.y = static_cast<LONG> (resolution.y) - mouse_position.y;
	
	mouse_client_position_ = Vector3{ static_cast<float>(mouse_position.x), static_cast<float>(mouse_position.y), 0.f };

	mouse_position.x += static_cast<LONG>(camera_position.x - resolution.x * 0.5f);
	mouse_position.y += static_cast<LONG>(camera_position.y - resolution.y * 0.5f);

	mouse_world_position_ = Vector3{ static_cast<float>(mouse_position.x), static_cast<float>(mouse_position.y), 0.f };
}

bool K::InputManager::KeyDown(std::string const& _tag) const
{
	return _FindKeyDesc(_tag).down;
}

bool K::InputManager::KeyPressed(std::string const& _tag) const
{
	return _FindKeyDesc(_tag).pressed;
}

bool K::InputManager::KeyUp(std::string const& _tag) const
{
	return _FindKeyDesc(_tag).up;
}

K::Vector3 const& K::InputManager::mouse_world_position() const
{
	return mouse_world_position_;
}

K::Vector3 const& K::InputManager::mouse_client_position() const
{
	return mouse_client_position_;
}

void K::InputManager::_Finalize()
{
}

K::KEY_DESC const& K::InputManager::_FindKeyDesc(std::string const& _tag) const
{
	auto iter = key_desc_map_.find(_tag);

	if (iter == key_desc_map_.end())
		return key_desc_dummy_.second;

	return iter->second;
}

void K::InputManager::_CreateKeyDesc()
{
	key_desc_map_.insert(std::move(key_desc_dummy_));
}
