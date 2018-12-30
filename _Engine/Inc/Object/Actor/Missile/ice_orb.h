#pragma once

#include "missile_actor.h"

namespace K
{
	class K_ENGINE_DLL IceOrb final : public MissileActor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

		void set_speed(float _speed);
		void set_range(float _range);
		void set_direction(Vector3 const& _direction);

	private:
		IceOrb() = default;
		IceOrb(IceOrb const& _other);
		IceOrb(IceOrb&& _other) noexcept;
		IceOrb& operator=(IceOrb const&) = delete;
		IceOrb& operator=(IceOrb&&) noexcept = default;

		virtual void _Finalize() override;

		virtual void _Update(float _time) override;

		bool once_flag_{ true };
		float speed_{};
		float range_{};
		float angle_{};
		float elapsed_time_{};
		float interval_time_{ 0.1f };
		Vector3 direction_{};
	};
}
