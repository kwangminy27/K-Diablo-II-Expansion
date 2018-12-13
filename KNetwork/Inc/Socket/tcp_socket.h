#pragma once

namespace K
{
	class SocketAddress;

	class K_NETWORK_DLL TCPSocket
	{
		friend class SocketManager;
	public:
		void Bind(SocketAddress const& _address);

		void Listen(int _backlog);
		std::shared_ptr<TCPSocket> Accept(SocketAddress& _from);

		void Connect(SocketAddress const& _to);

		int Send(void const* _buffer, int _size);
		int Receive(void* _buffer, int _size);

		void ShutDown();

	private:
		TCPSocket(SOCKET _socket);
		TCPSocket(TCPSocket&& _other) noexcept;
		TCPSocket& operator=(TCPSocket&&) noexcept = default;

		void _Finalize();

		SOCKET socket_{};
	};
}
