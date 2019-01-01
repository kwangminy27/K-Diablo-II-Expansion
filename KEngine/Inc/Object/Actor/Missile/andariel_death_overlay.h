#pragma once

#include "missile_actor.h"

namespace K
{
	class K_ENGINE_DLL AndarielDeathOverlay final : public MissileActor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

	private:
		AndarielDeathOverlay() = default;
		AndarielDeathOverlay(AndarielDeathOverlay const& _other);
		AndarielDeathOverlay(AndarielDeathOverlay&& _other) noexcept;
		AndarielDeathOverlay& operator=(AndarielDeathOverlay const&) = delete;
		AndarielDeathOverlay& operator=(AndarielDeathOverlay&&) noexcept = default;

		virtual void _Finalize() override;
	};
}
