#pragma once

#include "missile_actor.h"

namespace K
{
	class K_ENGINE_DLL AndarielSpecialCastOverlay final : public MissileActor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

	private:
		AndarielSpecialCastOverlay() = default;
		AndarielSpecialCastOverlay(AndarielSpecialCastOverlay const& _other);
		AndarielSpecialCastOverlay(AndarielSpecialCastOverlay&& _other) noexcept;
		AndarielSpecialCastOverlay& operator=(AndarielSpecialCastOverlay const&) = delete;
		AndarielSpecialCastOverlay& operator=(AndarielSpecialCastOverlay&&) noexcept = default;

		virtual void _Finalize() override;
	};
}
