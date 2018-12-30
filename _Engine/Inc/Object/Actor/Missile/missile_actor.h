#pragma once

#include "../actor.h"

namespace K
{
	class K_ENGINE_DLL MissileActor : public Actor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() = 0;

		virtual APTR Clone() const = 0;

		virtual void Serialize(InputMemoryStream& _imstream) = 0;
		virtual void Serialize(OutputMemoryStream& _omstream) = 0;

	protected:
		MissileActor() = default;
		MissileActor(MissileActor const& _other);
		MissileActor(MissileActor&& _other) noexcept;
		MissileActor& operator=(MissileActor const&) = delete;
		MissileActor& operator=(MissileActor&&) noexcept = default;

		virtual void _Finalize() = 0;
	};
}
