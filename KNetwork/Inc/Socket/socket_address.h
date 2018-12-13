#pragma once

namespace K
{
	class K_NETWORK_DLL SocketAddress
	{
		friend class UDPSocket;
		friend class TCPSocket;
	public:
		SocketAddress() = default;
		SocketAddress(uint32_t _ip_address, uint16_t _port);
		SocketAddress(sockaddr const& _sockaddr);

		int GetSize() const;

	private:
		sockaddr sockaddr_;
	};
}
