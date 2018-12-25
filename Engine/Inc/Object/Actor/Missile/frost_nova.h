#pragma once

#include "missile_actor.h"

namespace K
{
	class K_ENGINE_DLL FrostNova final : public MissileActor
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
		FrostNova() = default;
		FrostNova(FrostNova const& _other);
		FrostNova(FrostNova&& _other) noexcept;
		FrostNova& operator=(FrostNova const&) = delete;
		FrostNova& operator=(FrostNova&&) noexcept = default;

		virtual void _Finalize() override;

		virtual void _Update(float _time) override;

		float speed_{};
		float range_{};
		Vector3 direction_{};
	};
}
