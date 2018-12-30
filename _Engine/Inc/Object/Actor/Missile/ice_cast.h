#pragma once

#include "missile_actor.h"

namespace K
{
	class K_ENGINE_DLL IceCast final : public MissileActor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

	private:
		IceCast() = default;
		IceCast(IceCast const& _other);
		IceCast(IceCast&& _other) noexcept;
		IceCast& operator=(IceCast const&) = delete;
		IceCast& operator=(IceCast&&) noexcept = default;

		virtual void _Finalize() override;
	};
}
