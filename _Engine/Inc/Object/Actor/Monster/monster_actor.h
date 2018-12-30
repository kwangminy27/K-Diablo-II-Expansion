#pragma once

#include "../actor.h"

namespace K
{
	class K_ENGINE_DLL MonsterActor : public Actor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() = 0;

		virtual APTR Clone() const = 0;

		virtual void Serialize(InputMemoryStream& _imstream) = 0;
		virtual void Serialize(OutputMemoryStream& _omstream) = 0;

	protected:
		MonsterActor() = default;
		MonsterActor(MonsterActor const& _other);
		MonsterActor(MonsterActor&& _other) noexcept;
		MonsterActor& operator=(MonsterActor const&) = delete;
		MonsterActor& operator=(MonsterActor&&) noexcept = default;

		virtual void _Finalize() = 0;
	};
}
