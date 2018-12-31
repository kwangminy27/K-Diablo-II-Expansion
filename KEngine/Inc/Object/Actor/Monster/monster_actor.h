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

		float hp() const;
		float speed() const;
		Vector3 const& direction()  const;
		APTR target() const;

		void AddHp(float _value);

		void set_hp(float _hp);
		void set_speed(float _speed);
		void set_direction(Vector3 const& _direction);
		void set_target(APTR const& _actor);

	protected:
		MonsterActor() = default;
		MonsterActor(MonsterActor const& _other);
		MonsterActor(MonsterActor&& _other) noexcept;
		MonsterActor& operator=(MonsterActor const&) = delete;
		MonsterActor& operator=(MonsterActor&&) noexcept = default;

		virtual void _Finalize() = 0;

		float hp_{};
		float speed_{};
		Vector3 direction_{};
		std::weak_ptr<Actor> target_{};
	};
}
