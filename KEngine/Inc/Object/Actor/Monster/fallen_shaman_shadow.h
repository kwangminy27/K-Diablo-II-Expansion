#pragma once

#include "monster_actor.h"

namespace K
{
	class K_ENGINE_DLL FallenShamanShadow final : public MonsterActor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

	private:
		FallenShamanShadow() = default;
		FallenShamanShadow(FallenShamanShadow const& _other);
		FallenShamanShadow(FallenShamanShadow&& _other) noexcept;
		FallenShamanShadow& operator=(FallenShamanShadow const&) = delete;
		FallenShamanShadow& operator=(FallenShamanShadow&&) noexcept = default;

		virtual void _Finalize() override;

		virtual void _Render(float _time) override;
	};
}
