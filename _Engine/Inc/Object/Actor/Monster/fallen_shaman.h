#pragma once

#include "monster_actor.h"

namespace K
{
	class K_ENGINE_DLL FallenShaman final : public MonsterActor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

	private:
		FallenShaman() = default;
		FallenShaman(FallenShaman const& _other);
		FallenShaman(FallenShaman&& _other) noexcept;
		FallenShaman& operator=(FallenShaman const&) = delete;
		FallenShaman& operator=(FallenShaman&&) noexcept = default;

		virtual void _Finalize() override;

		virtual void _Input(float _time) override;
		virtual void _Update(float _time) override;
		virtual void _Render(float _time) override;
	};
}
