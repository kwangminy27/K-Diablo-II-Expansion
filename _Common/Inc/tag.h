#pragma once

namespace K
{
	enum class TAG_STATE
	{
		NORMAL,
		DISABLED,
		DEAD
	};

	class K_COMMON_DLL Tag
	{
	public:
		virtual void Initialize() = 0;

		TAG_STATE tag_state() const;
		TAG const& tag() const;

		void set_tag_state(TAG_STATE _state);
		void set_tag(TAG const& _tag);

	protected:
		Tag() = default;
		Tag(Tag const& _other);
		Tag(Tag&& _other) noexcept;
		Tag& operator=(Tag const&) = default;
		Tag& operator=(Tag&&) noexcept = default;

		virtual void _Finalize() = 0;

	private:
		TAG_STATE tag_state_{};
		TAG tag_{};
	};
}
