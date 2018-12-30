#pragma once

#include "missile_actor.h"

namespace K
{
	class K_ENGINE_DLL IceBolt final : public MissileActor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

		void set_speed(float _speed);
		void set_range(float _range);
		void set_spin_flag(bool _flag);
		void set_direction(Vector3 const& _direction);

	private:
		IceBolt() = default;
		IceBolt(IceBolt const& _other);
		IceBolt(IceBolt&& _other) noexcept;
		IceBolt& operator=(IceBolt const&) = delete;
		IceBolt& operator=(IceBolt&&) noexcept = default;

		virtual void _Finalize() override;

		virtual void _Update(float _time) override;

		float speed_{};
		float range_{};
		bool spin_flag_{};
		bool spin_point_1_{ true };
		bool spin_point_2_{ true };
		Vector3 direction_{};
	};
}
