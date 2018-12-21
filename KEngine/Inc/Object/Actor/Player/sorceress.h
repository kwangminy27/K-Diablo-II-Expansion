#pragma once

#include "player_actor.h"

namespace K
{
	class K_ENGINE_DLL Sorceress final : public PlayerActor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

	private:
		Sorceress() = default;
		Sorceress(Sorceress const& _other);
		Sorceress(Sorceress&& _other) noexcept;
		Sorceress& operator=(Sorceress const&) = delete;
		Sorceress& operator=(Sorceress&&) noexcept = default;

		virtual void _Finalize() override;
	};
}