#pragma once

#include "../actor.h"

namespace K
{
	class K_ENGINE_DLL PlayerActor : public Actor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() = 0;

		virtual APTR Clone() const = 0;

		virtual void Serialize(InputMemoryStream& _imstream) = 0;
		virtual void Serialize(OutputMemoryStream& _omstream) = 0;

		float hp() const;

		void AddHp(float _value);

		void set_hp(float _hp);
		void set_focus_flag(bool _flag);

	protected:
		PlayerActor() = default;
		PlayerActor(PlayerActor const& _other);
		PlayerActor(PlayerActor&& _other) noexcept;
		PlayerActor& operator=(PlayerActor const&) = delete;
		PlayerActor& operator=(PlayerActor&&) noexcept = default;

		virtual void _Finalize() = 0;

		std::array<bool, static_cast<int>(ACTOR_STATE::MAX)> once_flag_array_{};

		float hp_{};
		bool focus_flag_{};
		bool run_flag_{};
	};
}
