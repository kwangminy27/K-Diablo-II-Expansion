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

		void set_focus_flag(bool _flag);

	protected:
		PlayerActor() = default;
		PlayerActor(PlayerActor const& _other);
		PlayerActor(PlayerActor&& _other) noexcept;
		PlayerActor& operator=(PlayerActor const&) = delete;
		PlayerActor& operator=(PlayerActor&&) noexcept = default;

		virtual void _Finalize() = 0;

		bool focus_flag_{};
		bool run_flag_{};
	};
}
