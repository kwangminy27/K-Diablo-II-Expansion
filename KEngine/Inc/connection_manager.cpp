#include "KEngine.h"
#include "connection_manager.h"

#include "core.h"

std::unique_ptr<K::Connection> K::ConnectionManager::connection_dummy_{};

void K::ConnectionManager::Initialize()
{
	try
	{
		WSADATA data{};
		WSAStartup(MAKEWORD(2, 2), &data);

		for (auto i = 0; i < USER_SIZE; ++i)
			connection_vector_.push_back(std::make_unique<Connection>());
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "ConnectionManager::Initialize" << std::endl;
	}
}

void K::ConnectionManager::RunIOCP(std::function<void(int, uint8_t*)> const& _function)
{
	iocp_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, 0);

	for (auto& connection : connection_vector_)
	{
		connection->overlapped_ex.wsa_buf.buf = reinterpret_cast<char*>(connection->overlapped_ex.buffer);
		connection->overlapped_ex.wsa_buf.len = MTU_SIZE;
		connection->overlapped_ex.operation = IO_OPERATION::RECV;
	}

	std::vector<std::unique_ptr<std::thread>> worker_thread_vector{};

	for (auto i = 0; i < 6; ++i)
		worker_thread_vector.push_back(std::make_unique<std::thread>(WorkerThreadFunc, _function));
	
	std::unique_ptr<std::thread> accept_thread = std::make_unique<std::thread>(AcceptThreadFunc);

	auto const& core = Core::singleton();
	core->Run();

	std::terminate();
}

void K::ConnectionManager::HandlePacket(std::function<void(int, uint8_t*)> const& _function, int _id, uint8_t* _packet)
{
	_function(_id, _packet);
}

std::unique_ptr<K::Connection> const& K::ConnectionManager::FindConnection(int _id) const
{
	if (_id < 0 || _id >= connection_vector_.size())
		return connection_dummy_;

	return connection_vector_.at(_id);
}

void K::ConnectionManager::AddConnection(std::unique_ptr<Connection>& _connection)
{
	connection_vector_.at(_connection->id) = std::move(_connection);
}

void K::ConnectionManager::RemoveConnection(std::unique_ptr<Connection> const& _connection)
{
	connection_vector_.at(_connection->id) = nullptr;
}

HANDLE K::ConnectionManager::iocp() const
{
	return iocp_;
}

std::vector<std::unique_ptr<K::Connection>>& K::ConnectionManager::connection_vector()
{
	return connection_vector_;
}

void K::ConnectionManager::_Finalize()
{
	WSACleanup();
}

void K::WorkerThreadFunc(std::function<void(int, uint8_t*)> const& _function)
{
	auto const& connection_manager = ConnectionManager::singleton();

	auto& connection_vector = connection_manager->connection_vector();

	bool result{};
	DWORD io_size{};
	ULONG_PTR key{};
	OverlappedEx* overlapped_ex{};

	while (true)
	{
		result = GetQueuedCompletionStatus(connection_manager->iocp(), &io_size, &key, reinterpret_cast<LPOVERLAPPED*>(&overlapped_ex), INFINITE);

		if (false == result)
			throw std::exception{ "WorkerThreadFunc" };

		if (0 == io_size)
		{
			connection_vector.at(key)->tcp_socket = nullptr;
			connection_vector.at(key)->connection_flag = false;

			std::cout << "Client: " << connection_vector.at(key)->id << " logout" << std::endl;

			continue;
		}

		switch (overlapped_ex->operation)
		{
		case IO_OPERATION::RECV:
		{
			uint8_t* buffer = overlapped_ex->buffer;
			int remained = io_size;

			while (remained > 0)
			{
				if (0 == connection_vector.at(key)->packet_size)
					connection_vector.at(key)->packet_size = buffer[0];

				int required = connection_vector.at(key)->packet_size - connection_vector.at(key)->previous_data;

				if (remained >= required)
				{
					memcpy_s(connection_vector.at(key)->buffer + connection_vector.at(key)->previous_data, required, buffer, required);

					connection_manager->HandlePacket(_function, static_cast<int>(key), connection_vector.at(key)->buffer);

					connection_vector.at(key)->packet_size = 0;
					connection_vector.at(key)->previous_data = 0;

					buffer += required;
					remained -= required;
				}
				else
				{
					memcpy_s(connection_vector.at(key)->buffer + connection_vector.at(key)->previous_data, remained, buffer, remained);

					connection_vector.at(key)->previous_data += remained;

					remained = 0;
				}
			}

			DWORD flags{};
			if (0 != WSARecv(connection_vector.at(key)->tcp_socket->socket(), &connection_vector.at(key)->overlapped_ex.wsa_buf, 1, nullptr, &flags, &connection_vector.at(key)->overlapped_ex.wsa_overlapped, nullptr))
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
					throw std::exception{ "WorkerThreadFunc" };
			}
		}
			break;

		case IO_OPERATION::SEND:
			delete overlapped_ex;
			break;

		default:
			throw std::exception{ "WorkerThreadFunc" };
			break;
		}
	}
}

void K::AcceptThreadFunc()
{
	auto const& connection_manager = ConnectionManager::singleton();
	auto const& socket_manager = SocketManager::singleton();

	auto& connection_vector = connection_manager->connection_vector();

	auto listen_socket = socket_manager->CreateOverlappedTCPSocket();

	SocketAddress server_address{ INADDR_ANY, 21027 };

	listen_socket->Bind(server_address);
	listen_socket->Listen(10);

	while (true)
	{
		SocketAddress client_address{};
		auto new_socket = listen_socket->Accept(client_address);

		bool option = true;
		setsockopt(new_socket->socket(), IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char const*>(&option), sizeof(option));

		int new_id = -1;

		for (auto i = 0; i < connection_vector.size(); ++i)
		{
			if (false == connection_vector.at(i)->connection_flag)
			{
				new_id = i;
				break;
			}
		}

		if (-1 == new_id)
		{
			std::cout << "서버가 수용 가능한 인원을 초과했습니다." << std::endl;
			continue;
		}

		if (nullptr == CreateIoCompletionPort(reinterpret_cast<HANDLE>(new_socket->socket()), connection_manager->iocp(), new_id, NULL))
			throw std::exception{ "AcceptThreadFunc" };

		connection_vector.at(new_id)->id = new_id;
		connection_vector.at(new_id)->tcp_socket = new_socket;
		connection_vector.at(new_id)->connection_flag = true;

		std::cout << "Client " << new_id << " login" << std::endl;

		DWORD flags{};
		if (0 != WSARecv(connection_vector.at(new_id)->tcp_socket->socket(), &connection_vector.at(new_id)->overlapped_ex.wsa_buf, 1, nullptr, &flags, &connection_vector.at(new_id)->overlapped_ex.wsa_overlapped, nullptr))
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
				throw std::exception{ "AcceptThreadFunc" };
		}
	}
}
