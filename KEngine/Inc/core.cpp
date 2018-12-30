#include "KEngine.h"
#include "core.h"

#include "device_manager.h"
#include "text_manager.h"
#include "path_manager.h"
#include "Resource/resource_manager.h"
#include "Rendering/rendering_manager.h"
#include "Audio/audio_manager.h"
#include "Video/video_manager.h"
#include "time_manager.h"
#include "input_manager.h"
#include "World/world_manager.h"
#include "Object/object_manager.h"
#include "Object/Actor/camera_actor.h"
#include "collision_manager.h"
#include "navigation_manager.h"
#include "registry_manager.h"
#include "replication_manager.h"
#include "connection_manager.h"

bool K::Core::shutdown_{};
bool K::Core::chat_{};
bool K::Core::hangul_{};
std::wstring K::Core::chat_message_{};

void K::Core::Initialize()
{
}

void K::Core::Initialize(std::wstring const& _class_name, std::wstring const& _window_name, HINSTANCE _instance)
{
	try
	{
		_RegisterClass(_class_name);
		_CreateWindow(_class_name, _window_name);

		Initialize(_instance, window_);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Core::Initialize" << std::endl;
	}
}

void K::Core::Initialize(HINSTANCE _instance, HWND _window)
{
	try
	{
		ThrowIfFailed(CoInitializeEx(nullptr, COINIT_MULTITHREADED));

		instance_ = _instance;
		window_ = _window;

		SocketManager::singleton()->Initialize();

		DeviceManager::singleton()->Initialize(window_);
		TextManager::singleton()->Initialize();
		PathManager::singleton()->Initialize();
		VideoManager::singleton()->Initialize();
		AudioManager::singleton()->Initialize();
		ResourceManager::singleton()->Initialize();
		RenderingManager::singleton()->Initialize();
		TimeManager::singleton()->Initialize();
		InputManager::singleton()->Initialize();
		WorldManager::singleton()->Initialize();
		ObjectManager::singleton()->Initialize();
		CollisionManager::singleton()->Initialize();
		NavigationManager::singleton()->Initialize();
		RegistryManager::singleton()->Initialize();
		ReplicationManager::singleton()->Initialize();
		ConnectionManager::singleton()->Initialize();
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Core::Initialize" << std::endl;
	}
}

void K::Core::RunServer(std::function<void(int, uint8_t*)> const& _function)
{
	auto const& connection_manager = ConnectionManager::singleton();

	connection_manager->RunIOCP(_function);
}

void ChatFunc(std::shared_ptr<K::TCPSocket> const& _socket)
{
	std::wstring message = K::Core::chat_message();

	K::CS_PACKET_CHAT cs_packet_chat{};
	cs_packet_chat.size = sizeof(K::CS_PACKET_CHAT);
	cs_packet_chat.type = K::CS_CHAT;
	for (auto i = 0; i < message.size(); ++i)
	{
		wchar_t temp = message.at(i);
		temp = htons(temp);
		memcpy_s(cs_packet_chat.message + i, sizeof(wchar_t), &temp, sizeof(wchar_t));
	}

	_socket->Send(&cs_packet_chat, sizeof(K::CS_PACKET_CHAT));

	K::Core::chat_message().clear();
}

void K::Core::RunClient(std::function<void(int, uint8_t*)> const& _function)
{
	auto const& connection_manager = ConnectionManager::singleton();

	auto const& socket_manager = SocketManager::singleton();

	auto client_socket = socket_manager->CreateTCPSocket();

	uint32_t ip_address{};
	InetPton(AF_INET, L"192.168.219.103", &ip_address);
	SocketAddress server_address{ ntohl(ip_address), 21027 };

	client_socket->Connect(server_address);

	client_socket->SetNonBlockingMode(true);

	CS_PACKET_LOGIN cs_packet_login{};
	cs_packet_login.size = sizeof(cs_packet_login);
	cs_packet_login.type = K::CS_LOGIN;
	client_socket->Send(&cs_packet_login, sizeof(CS_PACKET_LOGIN));

	uint8_t packet_buffer[MTU_SIZE]{};
	uint32_t packet_size{};
	uint32_t previous_data{};

	std::unique_ptr<std::thread> chat_thread{};

	MSG message{};
	while (false == shutdown_)
	{
		if (PeekMessage(&message, nullptr, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			Logic();

			auto const& input_manager = InputManager::singleton();

			if (input_manager->KeyDown("Enter"))
			{
				if (chat_)
				{
					chat_thread = std::make_unique<std::thread>(ChatFunc, client_socket);
					chat_thread->detach();

					chat_ = false;
				}
				else
					chat_ = true;
			}

			uint8_t buffer[MTU_SIZE]{};
			uint8_t *buffer_ptr = buffer;

			int io_size = client_socket->Receive(buffer, MTU_SIZE);

			int remained = io_size;

			while (remained > 0)
			{
				if (0 == packet_size)
					packet_size = *(reinterpret_cast<uint32_t*>(buffer));

				int required = packet_size - previous_data;

				if (remained >= required)
				{
					memcpy_s(packet_buffer + previous_data, required, buffer_ptr, required);

					connection_manager->HandlePacket(_function, -1, packet_buffer);

					packet_size = 0;
					previous_data = 0;

					buffer_ptr += required;
					remained -= required;
				}
				else
				{
					memcpy_s(packet_buffer + previous_data, remained, buffer_ptr, remained);

					previous_data += remained;

					remained = 0;
				}
			}
		}
	}

	client_socket->ShutDown();
}

void K::Core::Run()
{
	MSG message{};
	while (!shutdown_)
	{
		if (PeekMessage(&message, nullptr, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
			Logic();
	}
}

void K::Core::Logic()
{
	auto const& time_manager = TimeManager::singleton();
	auto const& input_manager = InputManager::singleton();

	time_manager->Update();
	input_manager->Update();

	static bool pause = true;
	if (input_manager->KeyDown("F7"))
		pause ^= true;

	static bool caps_lock{};
	if (input_manager->KeyDown("CapsLock"))
		caps_lock ^= true;

	static bool shift{};
	if (input_manager->KeyPressed("LShift") || input_manager->KeyPressed("RShift"))
		shift = true;
	else
		shift = false;

	// 한글은 아직 미구현
	if (input_manager->KeyDown("Hangul"))
		hangul_ ^= true;

	float time_delta = time_manager->time_delta() * pause;

	if(false == chat_)
		_Input(time_delta);
	else if(chat_ && false == hangul_)
	{
		if (input_manager->KeyDown("`"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('~');
			else
				chat_message_.push_back('`');
		}
		if (input_manager->KeyDown("1"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('!');
			else
				chat_message_.push_back('1');
		}
		if (input_manager->KeyDown("2"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('@');
			else
				chat_message_.push_back('2');
		}
		if (input_manager->KeyDown("3"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('#');
			else
				chat_message_.push_back('3');
		}
		if (input_manager->KeyDown("4"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('$');
			else
				chat_message_.push_back('4');
		}
		if (input_manager->KeyDown("5"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('%');
			else
				chat_message_.push_back('5');
		}
		if (input_manager->KeyDown("6"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('^');
			else
				chat_message_.push_back('6');
		}
		if (input_manager->KeyDown("7"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('&');
			else
				chat_message_.push_back('7');
		}
		if (input_manager->KeyDown("8"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('*');
			else
				chat_message_.push_back('8');
		}
		if (input_manager->KeyDown("9"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('(');
			else
				chat_message_.push_back('9');
		}
		if (input_manager->KeyDown("0"))
		{
			if (caps_lock || shift)
				chat_message_.push_back(')');
			else
				chat_message_.push_back('0');
		}
		if (input_manager->KeyDown("-"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('_');
			else
				chat_message_.push_back('-');
		}
		if (input_manager->KeyDown("="))
		{
			if (caps_lock || shift)
				chat_message_.push_back('+');
			else
				chat_message_.push_back('=');
		}
		if (input_manager->KeyDown("["))
		{
			if (caps_lock || shift)
				chat_message_.push_back('{');
			else
				chat_message_.push_back('[');
		}
		if (input_manager->KeyDown("]"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('}');
			else
				chat_message_.push_back(']');
		}
		if (input_manager->KeyDown("\\"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('|');
			else
				chat_message_.push_back('\\');
		}
		if (input_manager->KeyDown(";"))
		{
			if (caps_lock || shift)
				chat_message_.push_back(':');
			else
				chat_message_.push_back(';');
		}
		if (input_manager->KeyDown("'"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('"');
			else
				chat_message_.push_back('\'');
		}
		if (input_manager->KeyDown(","))
		{
			if (caps_lock || shift)
				chat_message_.push_back('<');
			else
				chat_message_.push_back(',');
		}
		if (input_manager->KeyDown("."))
		{
			if (caps_lock || shift)
				chat_message_.push_back('>');
			else
				chat_message_.push_back('.');
		}
		if (input_manager->KeyDown("/"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('?');
			else
				chat_message_.push_back('/');
		}
		if (input_manager->KeyDown("Q"))
		{
			if(caps_lock || shift)
				chat_message_.push_back('Q');
			else
				chat_message_.push_back('q');
		}
		if (input_manager->KeyDown("W"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('W');
			else
				chat_message_.push_back('w');
		}
		if (input_manager->KeyDown("E"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('E');
			else
				chat_message_.push_back('e');
		}
		if (input_manager->KeyDown("R"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('R');
			else
				chat_message_.push_back('r');
		}
		if (input_manager->KeyDown("T"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('T');
			else
				chat_message_.push_back('t');
		}
		if (input_manager->KeyDown("Y"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('Y');
			else
				chat_message_.push_back('y');
		}
		if (input_manager->KeyDown("U"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('U');
			else
				chat_message_.push_back('u');
		}
		if (input_manager->KeyDown("I"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('I');
			else
				chat_message_.push_back('i');
		}
		if (input_manager->KeyDown("O"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('O');
			else
				chat_message_.push_back('o');
		}
		if (input_manager->KeyDown("P"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('P');
			else
				chat_message_.push_back('p');
		}
		if (input_manager->KeyDown("A"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('A');
			else
				chat_message_.push_back('a');
		}
		if (input_manager->KeyDown("S"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('S');
			else
				chat_message_.push_back('s');
		}
		if (input_manager->KeyDown("D"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('D');
			else
				chat_message_.push_back('d');
		}
		if (input_manager->KeyDown("F"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('F');
			else
				chat_message_.push_back('f');
		}
		if (input_manager->KeyDown("G"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('G');
			else
				chat_message_.push_back('g');
		}
		if (input_manager->KeyDown("H"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('H');
			else
				chat_message_.push_back('h');
		}
		if (input_manager->KeyDown("J"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('J');
			else
				chat_message_.push_back('j');
		}
		if (input_manager->KeyDown("K"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('K');
			else
				chat_message_.push_back('k');
		}
		if (input_manager->KeyDown("L"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('L');
			else
				chat_message_.push_back('l');
		}
		if (input_manager->KeyDown("Z"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('Z');
			else
				chat_message_.push_back('z');
		}
		if (input_manager->KeyDown("X"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('X');
			else
				chat_message_.push_back('x');
		}
		if (input_manager->KeyDown("C"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('C');
			else
				chat_message_.push_back('c');
		}
		if (input_manager->KeyDown("V"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('V');
			else
				chat_message_.push_back('v');
		}
		if (input_manager->KeyDown("B"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('B');
			else
				chat_message_.push_back('b');
		}
		if (input_manager->KeyDown("N"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('N');
			else
				chat_message_.push_back('n');
		}
		if (input_manager->KeyDown("M"))
		{
			if (caps_lock || shift)
				chat_message_.push_back('M');
			else
				chat_message_.push_back('m');
		}

		if (input_manager->KeyDown("Space"))
			chat_message_.push_back(' ');

		static float elapsed_time{};
		if (input_manager->KeyPressed("Back"))
		{
			if (false == chat_message_.empty())
			{
				elapsed_time += time_delta;

				if (elapsed_time >= 0.1f)
				{
					chat_message_.pop_back();

					elapsed_time -= 0.1f;
				}
			}
		}
	}

	_Update(time_delta);
	_Collision(time_delta);
	_Render(time_delta);
}

bool K::Core::shutdown()
{
	return shutdown_;
}

bool K::Core::chat()
{
	return chat_;
}

std::wstring& K::Core::chat_message()
{
	return chat_message_;
}

HWND K::Core::window() const
{
	return window_;
}

void K::Core::_Finalize()
{
	ConnectionManager::singleton().reset();
	ReplicationManager::singleton().reset();
	RegistryManager::singleton().reset();
	NavigationManager::singleton().reset();
	CollisionManager::singleton().reset();
	InputManager::singleton().reset();
	TimeManager::singleton().reset();
	ObjectManager::singleton().reset();
	WorldManager::singleton().reset();
	RenderingManager::singleton().reset();
	ResourceManager::singleton().reset();
	AudioManager::singleton().reset();
	VideoManager::singleton().reset();
	PathManager::singleton().reset();
	TextManager::singleton().reset();
	DeviceManager::singleton().reset();

	SocketManager::singleton().reset();

	CoUninitialize();
}

LRESULT K::Core::_WindowProc(HWND _window, UINT _message, WPARAM _w_param, LPARAM _l_param)
{
	switch (_message)
	{
	case WM_CHAR:
	{
		if (chat_ && hangul_)
		{
			if (VK_SPACE == _w_param)
				chat_message_.push_back(' ');
			else if (VK_BACK == _w_param)
			{
				if (false == chat_message_.empty())
					chat_message_.pop_back();
			}
			else
				chat_message_.push_back(static_cast<wchar_t>(_w_param));
		}
	}
		return 0;

	case WM_MOUSEWHEEL:
	{
		static float scale = 1.f;

		auto result = static_cast<short>(HIWORD(_w_param));
	
		auto const& default_camera = WorldManager::singleton()->FindCamera(TAG{ DEFAULT_CAMERA, 0 });

		if (120 == result)
			scale -= .2f;
		else
			scale += .2f;

		scale = std::clamp(scale, 0.2f, 5.f);

		default_camera->set_scale(scale);
		default_camera->CreateProjection(static_cast<float>(RESOLUTION::WIDTH) * scale, static_cast<float>(RESOLUTION::HEIGHT) * scale, 0.f, 1000.f);
	}
		return 0;

	case WM_DESTROY:
		Core::shutdown_ = true;
		AudioManager::singleton()->Suspend();
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(_window, _message, _w_param, _l_param);
}

void K::Core::_RegisterClass(std::wstring const& _class_name)
{
	WNDCLASSEX wcex{};
	wcex.cbSize = sizeof(wcex);
	wcex.lpfnWndProc = K::Core::_WindowProc;
	wcex.hInstance = instance_;
	wcex.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wcex.lpszClassName = _class_name.c_str();
	if (!RegisterClassEx(&wcex))
		throw std::exception{ "Core::_RegisterClass" };
}

void K::Core::_CreateWindow(std::wstring const& _class_name, std::wstring const& _window_name)
{
	window_ = CreateWindowEx(
		NULL,
		_class_name.c_str(),
		_window_name.c_str(),
		WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		nullptr,
		nullptr,
		instance_,
		nullptr
	);

	if (!window_)
		throw std::exception{ "Core::_CreateWindow" };

	RECT rc{ 0, 0, static_cast<int>(RESOLUTION::WIDTH) , static_cast<int>(RESOLUTION::HEIGHT) };
	AdjustWindowRectEx(&rc, WS_CAPTION | WS_SYSMENU, false, NULL);

	MoveWindow(window_, 152, 70, rc.right - rc.left, rc.bottom - rc.top, true);
	ShowWindow(window_, SW_SHOW);
}

void K::Core::_Input(float _time)
{
	WorldManager::singleton()->Input(_time);
}

void K::Core::_Update(float _time)
{
	WorldManager::singleton()->Update(_time);
}

void K::Core::_Collision(float _time)
{
	WorldManager::singleton()->Collision(_time);
}

void K::Core::_Render(float _time)
{
	auto const& device_manager = DeviceManager::singleton();

	device_manager->Clear();

	WorldManager::singleton()->Render(_time);

	device_manager->Present();
}
