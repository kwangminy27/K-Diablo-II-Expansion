#pragma once

namespace K
{
	class TileSetLevel final : public Level
	{
		friend class WorldManager;
	public:
		virtual void Initialize() override;

	private:
		TileSetLevel() = default;
		TileSetLevel(TileSetLevel const&) = delete;
		TileSetLevel(TileSetLevel&& _other) noexcept;
		TileSetLevel& operator=(TileSetLevel const&) = delete;
		TileSetLevel& operator=(TileSetLevel&&) noexcept = default;

		virtual void _Finalize() override;

		virtual void _Input(float _time) override;
		virtual void _Update(float _time) override;
		virtual void _Render(float _time) override;
	};
}
