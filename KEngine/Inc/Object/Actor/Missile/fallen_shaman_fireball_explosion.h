#pragma once

#include "missile_actor.h"

namespace K
{
	class K_ENGINE_DLL FallenShamanFireballExplosion final : public MissileActor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

	private:
		FallenShamanFireballExplosion() = default;
		FallenShamanFireballExplosion(FallenShamanFireballExplosion const& _other);
		FallenShamanFireballExplosion(FallenShamanFireballExplosion&& _other) noexcept;
		FallenShamanFireballExplosion& operator=(FallenShamanFireballExplosion const&) = delete;
		FallenShamanFireballExplosion& operator=(FallenShamanFireballExplosion&&) noexcept = default;

		virtual void _Finalize() override;
	};
}
