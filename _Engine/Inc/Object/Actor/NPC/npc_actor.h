#pragma once

#include "../actor.h"

namespace K
{
	class K_ENGINE_DLL NPCActor : public Actor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() = 0;

		virtual APTR Clone() const = 0;

		virtual void Serialize(InputMemoryStream& _imstream) = 0;
		virtual void Serialize(OutputMemoryStream& _omstream) = 0;

	protected:
		NPCActor() = default;
		NPCActor(NPCActor const& _other);
		NPCActor(NPCActor&& _other) noexcept;
		NPCActor& operator=(NPCActor const&) = delete;
		NPCActor& operator=(NPCActor&&) noexcept = default;

		virtual void _Finalize() = 0;
	};
}
