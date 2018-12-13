#pragma once

namespace K
{
	class SocketAddress;

	class K_NETWORK_DLL UDPSocket
	{
		friend class SocketManager;
	public:
		void Bind(SocketAddress const& _address);

		int SendTo(void const* _buffer, int _size, SocketAddress const& _to);
		int ReceiveFrom(void* _buffer, int _size, SocketAddress& _from);

		void ShutDown();

	private:
		UDPSocket(SOCKET _socket);
		UDPSocket(UDPSocket&& _other) noexcept;
		UDPSocket& operator=(UDPSocket&&) noexcept = default;

		void _Finalize();

		SOCKET socket_{};
	};
}
