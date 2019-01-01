#pragma once

#include "missile_actor.h"

namespace K
{
	class K_ENGINE_DLL FallenShamanMissile final : public MissileActor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

	private:
		FallenShamanMissile() = default;
		FallenShamanMissile(FallenShamanMissile const& _other);
		FallenShamanMissile(FallenShamanMissile&& _other) noexcept;
		FallenShamanMissile& operator=(FallenShamanMissile const&) = delete;
		FallenShamanMissile& operator=(FallenShamanMissile&&) noexcept = default;

		virtual void _Finalize() override;
	};
}
