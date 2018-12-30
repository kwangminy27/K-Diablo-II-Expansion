#pragma once

#include "monster_actor.h"

namespace K
{
	class K_ENGINE_DLL Cow final : public MonsterActor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

	private:
		Cow() = default;
		Cow(Cow const& _other);
		Cow(Cow&& _other) noexcept;
		Cow& operator=(Cow const&) = delete;
		Cow& operator=(Cow&&) noexcept = default;

		virtual void _Finalize() override;
	};
}
