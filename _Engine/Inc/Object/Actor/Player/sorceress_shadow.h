#pragma once

#include "player_actor.h"

namespace K
{
	class K_ENGINE_DLL SorceressShadow final : public PlayerActor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

	private:
		SorceressShadow() = default;
		SorceressShadow(SorceressShadow const& _other);
		SorceressShadow(SorceressShadow&& _other) noexcept;
		SorceressShadow& operator=(SorceressShadow const&) = delete;
		SorceressShadow& operator=(SorceressShadow&&) noexcept = default;

		virtual void _Finalize() override;

		virtual void _Render(float _time) override;
	};
}
