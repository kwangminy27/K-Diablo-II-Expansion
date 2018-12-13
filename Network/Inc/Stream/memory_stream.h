#pragma once

namespace K
{
	enum class MEMORY_STREAM_TYPE
	{
		INPUT,
		OUTPUT
	};

	class K_NETWORK_DLL MemoryStream
	{
	public:
		MemoryStream();
		MemoryStream(MemoryStream const&) = delete;
		MemoryStream(MemoryStream&&) noexcept = delete;
		MemoryStream& operator=(MemoryStream const&) = delete;
		MemoryStream& operator=(MemoryStream&&) noexcept = delete;

		template <typename T> void Serialize(T& _data);
		template <> void Serialize(std::string& _data);
		template <> void Serialize(std::wstring& _data);
		template <typename T> void Serialize(std::vector<T>& _data_vector);

		void Clear();

		std::shared_ptr<std::vector<uint8_t>> const& buffer() const;
		uint32_t head() const;

	protected:
		std::shared_ptr<std::vector<uint8_t>> buffer_{};
		uint32_t head_{};

	private:
		virtual MEMORY_STREAM_TYPE _GetType() const = 0;
		virtual void _Serialize(void* _data, uint32_t _size) = 0;

		template <typename T> T _ByteSwap(T _data);
	};

	class K_NETWORK_DLL InputMemoryStream final : public MemoryStream
	{
	public:
		InputMemoryStream() = default;
		InputMemoryStream(InputMemoryStream const&) = delete;
		InputMemoryStream(InputMemoryStream&&) noexcept = delete;
		InputMemoryStream& operator=(InputMemoryStream const&) = delete;
		InputMemoryStream& operator=(InputMemoryStream&&) noexcept = delete;

	private:
		virtual MEMORY_STREAM_TYPE _GetType() const override;
		virtual void _Serialize(void* _data, uint32_t _size) override;

		void _Read(void* _data, uint32_t _size);
	};

	class K_NETWORK_DLL OutputMemoryStream final : public MemoryStream
	{
	public:
		OutputMemoryStream() = default;
		OutputMemoryStream(OutputMemoryStream const&) = delete;
		OutputMemoryStream(OutputMemoryStream&&) noexcept = delete;
		OutputMemoryStream& operator=(OutputMemoryStream const&) = delete;
		OutputMemoryStream& operator=(OutputMemoryStream&&) noexcept = delete;

	private:
		virtual MEMORY_STREAM_TYPE _GetType() const override;
		virtual void _Serialize(void* _data, uint32_t _size) override;

		void _Write(void const* _data, uint32_t _size);
	};

#pragma region ByteSwapper
	template <typename T, uint32_t Size> class ByteSwapper;

	template <typename T>
	class ByteSwapper<T, 1>
	{
	public:
		T Swap(T _data) const;
	};

	template <typename T>
	class ByteSwapper<T, 2>
	{
	public:
		T Swap(T _data) const;
	};

	template <typename T>
	class ByteSwapper<T, 4>
	{
	public:
		T Swap(T _data) const;
	};

	template <typename T>
	class ByteSwapper<T, 8>
	{
	public:
		T Swap(T _data) const;
	};
#pragma endregion
}

#include "memory_stream.inl"
