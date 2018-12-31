#include "KServer.h"
#include "Level/default_level.h"

int WINAPI wWinMain(HINSTANCE _instance, HINSTANCE _prev_instance, PWSTR _cmd_line, int _cmd_show)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	auto const& core = K::Core::singleton();
	auto const& world_manager = K::WorldManager::singleton();

	core->Initialize(L"K Game Engine", L"K Game Engine", _instance);

	world_manager->CreateLevel<K::DefaultLevel>({ "DefaultLevel", 0 });

	core->RunServer([](int _id, uint8_t* _packet) {
		auto const& connection_manager = K::ConnectionManager::singleton();
		auto const& replication_manager = K::ReplicationManager::singleton();

		uint8_t type = _packet[1];

		if (K::CS_LOGIN == type)
		{
			//std::cout << "CS_LOGIN" << std::endl;

			K::CS_PACKET_LOGIN cs_packet_login = *(reinterpret_cast<K::CS_PACKET_LOGIN*>(_packet));

			std::wstring message = L"[System] Client " + std::to_wstring(_id) + L"��(��) �����߽��ϴ�.";

			K::SC_PACKET_CHAT sc_packet_chat{};
			sc_packet_chat.size = static_cast<uint8_t>(sizeof(K::SC_PACKET_CHAT));
			sc_packet_chat.type = K::SC_CHAT;
			sc_packet_chat.id = _id;

			message.resize(K::MAX_MESSAGE_SIZE);
			message.at(K::MAX_MESSAGE_SIZE - 1) = '\0';

			for (auto i = 0; i < message.size(); ++i)
			{
				wchar_t temp = message.at(i);
				temp = htons(temp);
				memcpy_s(sc_packet_chat.message + i, sizeof(wchar_t), &temp, sizeof(wchar_t));
			}

			for (auto const& connection : connection_manager->connection_vector())
			{
				if (connection->connection_flag)
					connection->tcp_socket->Send(&sc_packet_chat, sizeof(K::SC_PACKET_CHAT));
			}

			replication_manager->SendActor(_id);
		}
		else if (K::CS_CHAT == type)
		{
			//std::cout << "CS_CHAT" << std::endl;

			K::CS_PACKET_CHAT cs_packet_chat = *(reinterpret_cast<K::CS_PACKET_CHAT*>(_packet));

			std::wstring client_message = cs_packet_chat.message;
			
			client_message.resize(K::MAX_MESSAGE_SIZE);
			client_message.at(K::MAX_MESSAGE_SIZE - 1) = '\0';

			std::wstring swapped_message{};
			for (auto i = 0; i < client_message.size(); ++i)
			{
				wchar_t temp = client_message.at(i);
				temp = ntohs(temp);
				swapped_message.push_back(temp);
			}

			std::wstring message = L"Client " + std::to_wstring(_id) + L": " + swapped_message;
			
			K::SC_PACKET_CHAT sc_packet_chat{};
			sc_packet_chat.size = static_cast<uint8_t>(sizeof(K::SC_PACKET_CHAT));
			sc_packet_chat.type = K::SC_CHAT;
			sc_packet_chat.id = _id;

			message.resize(K::MAX_MESSAGE_SIZE);
			message.at(K::MAX_MESSAGE_SIZE - 1) = '\0';

			for (auto i = 0; i < message.size(); ++i)
			{
				wchar_t temp = message.at(i);
				temp = htons(temp);
				memcpy_s(sc_packet_chat.message + i, sizeof(wchar_t), &temp, sizeof(wchar_t));
			}

			for (auto const& connection : connection_manager->connection_vector())
			{
				if(connection->connection_flag)
					connection->tcp_socket->Send(&sc_packet_chat, sizeof(K::SC_PACKET_CHAT));
			}
		}
		else
			std::cout << "Ÿ���� �� �� ���� ��Ŷ�� �����߽��ϴ�." << std::endl;
	});

	K::Core::singleton().reset();

	return 0;
}
