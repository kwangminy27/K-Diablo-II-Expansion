#pragma once

namespace K
{
	enum class EDITOR_STATE
	{
		NONE,
		TILE,
		OPTION,
		ACTOR
	};

	class DefaultLevel final : public Level
	{
		friend class WorldManager;
	public:
		virtual void Initialize() override;

	private:
		DefaultLevel() = default;
		DefaultLevel(DefaultLevel const&) = delete;
		DefaultLevel(DefaultLevel&& _other) noexcept;
		DefaultLevel& operator=(DefaultLevel const&) = delete;
		DefaultLevel& operator=(DefaultLevel&&) noexcept = default;

		virtual void _Finalize() override;

		virtual void _Input(float _time) override;
		virtual void _Update(float _time) override;
		virtual void _Render(float _time) override;

		EDITOR_STATE state_{};
	};
}
