#pragma once

#include "missile_actor.h"

namespace K
{
	class K_ENGINE_DLL FrozenArmor final : public MissileActor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

		void set_target(std::shared_ptr<Actor> const& _target);

	private:
		FrozenArmor() = default;
		FrozenArmor(FrozenArmor const& _other);
		FrozenArmor(FrozenArmor&& _other) noexcept;
		FrozenArmor& operator=(FrozenArmor const&) = delete;
		FrozenArmor& operator=(FrozenArmor&&) noexcept = default;

		virtual void _Finalize() override;

		virtual void _Update(float _time) override;

		float life_time_{};
		std::weak_ptr<Actor> target_{};
	};
}
