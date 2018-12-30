#pragma once

namespace K
{
	enum class IO_OPERATION
	{
		RECV,
		SEND
	};

	struct OverlappedEx
	{
		WSAOVERLAPPED wsa_overlapped;
		WSABUF wsa_buf;
		IO_OPERATION operation;
		uint8_t buffer[MTU_SIZE];
	};

	struct Connection
	{
		int id;
		bool connection_flag;
		uint32_t packet_size;
		uint32_t previous_data;
		uint8_t buffer[MTU_SIZE];
		OverlappedEx overlapped_ex;
		InputMemoryStream imstream;
		OutputMemoryStream omstream;
		std::shared_ptr<TCPSocket> tcp_socket;
	};

	void WorkerThreadFunc(std::function<void(int, uint8_t*)> const& _function);
	void AcceptThreadFunc();

	class K_ENGINE_DLL ConnectionManager : public Singleton<ConnectionManager>
	{
		friend class Singleton<ConnectionManager>;
	public:
		virtual void Initialize() override;

		void RunIOCP(std::function<void(int, uint8_t*)> const& _function);

		void HandlePacket(std::function<void(int, uint8_t*)> const& _function, int _id, uint8_t* _packet);
		void HandlePacket(std::function<void(int, uint8_t*)> const& _function, uint8_t* _packet);

		std::unique_ptr<Connection> const& FindConnection(int _id) const;

		void AddConnection(std::unique_ptr<Connection>& _connection);
		void RemoveConnection(std::unique_ptr<Connection> const& _connection);

		HANDLE iocp() const;
		std::vector<std::unique_ptr<Connection>>& connection_vector();

		static std::unique_ptr<Connection> connection_dummy_;

	private:
		ConnectionManager() = default;
		ConnectionManager(ConnectionManager const&) = delete;
		ConnectionManager(ConnectionManager&&) noexcept = delete;
		ConnectionManager& operator=(ConnectionManager const&) = delete;
		ConnectionManager& operator=(ConnectionManager&&) noexcept = delete;

		virtual void _Finalize() override;

		HANDLE iocp_{};
		std::vector<std::unique_ptr<Connection>> connection_vector_{};
	};
}
