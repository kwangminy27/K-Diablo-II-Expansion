#pragma once

#include "monster_actor.h"

namespace K
{
	class K_ENGINE_DLL WendigoShadow final : public MonsterActor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

	private:
		WendigoShadow() = default;
		WendigoShadow(WendigoShadow const& _other);
		WendigoShadow(WendigoShadow&& _other) noexcept;
		WendigoShadow& operator=(WendigoShadow const&) = delete;
		WendigoShadow& operator=(WendigoShadow&&) noexcept = default;

		virtual void _Finalize() override;

		virtual void _Render(float _time) override;
	};
}
