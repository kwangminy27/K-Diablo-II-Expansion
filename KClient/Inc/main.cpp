#include "KClient.h"
#include "Level/default_level.h"

#include "Actor/text_actor.h"

std::list<std::wstring> wide_message_list_{};

int WINAPI wWinMain(HINSTANCE _instance, HINSTANCE _prev_instance, PWSTR _cmd_line, int _cmd_show)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	auto const& core = K::Core::singleton();
	auto const& world_manager = K::WorldManager::singleton();

	core->Initialize(L"K Game Engine", L"K Game Engine", _instance);

	world_manager->CreateLevel<K::DefaultLevel>({ "DefaultLevel", 0 });

	core->RunClient([](int _id, uint8_t* _packet) {
		uint8_t type = _packet[4];

		if (K::SC_CHAT == type)
		{
			K::SC_PACKET_CHAT sc_packet_chat = *(reinterpret_cast<K::SC_PACKET_CHAT*>(_packet));

			std::wstring message = sc_packet_chat.message;

			std::wstring swapped_message{};
			for (auto i = 0; i < message.size(); ++i)
			{
				wchar_t temp = message.at(i);
				temp = ntohs(temp);
				swapped_message.push_back(temp);
			}

			wide_message_list_.push_front(swapped_message);

			if (wide_message_list_.size() > 5)
				wide_message_list_.pop_back();

			int counter{};
			for (auto const& e : wide_message_list_)
			{
				++counter;

				auto const& text = K::WorldManager::singleton()->FindActor(K::TAG{ "TextActor", counter });

				if (std::string::npos != e.find(L"[System]"))
					K::APTR_CAST<K::TextActor>(text)->set_color(DirectX::Colors::OrangeRed.v);
				else
					K::APTR_CAST<K::TextActor>(text)->set_color(DirectX::Colors::MistyRose.v);

				K::APTR_CAST<K::TextActor>(text)->set_text(e);
			}
		}
		else
			std::cout << "타입을 알 수 없는 패킷이 도착했습니다." << std::endl;
	});

	K::Core::singleton().reset();

	return 0;
}
