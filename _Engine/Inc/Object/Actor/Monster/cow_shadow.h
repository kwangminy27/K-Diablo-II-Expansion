#pragma once

#include "monster_actor.h"

namespace K
{
	class K_ENGINE_DLL CowShadow final : public MonsterActor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

	private:
		CowShadow() = default;
		CowShadow(CowShadow const& _other);
		CowShadow(CowShadow&& _other) noexcept;
		CowShadow& operator=(CowShadow const&) = delete;
		CowShadow& operator=(CowShadow&&) noexcept = default;

		virtual void _Finalize() override;

		virtual void _Render(float _time) override;
	};
}
