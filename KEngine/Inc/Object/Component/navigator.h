#pragma once

#include "component.h"

namespace K
{
	class TileMapActor;

	class K_ENGINE_DLL Navigator final : public Component
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual void Update(float _time);

		virtual CPTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

		void Route(Vector3 const& _start, Vector3 const& _end);

		bool move_flag() const;
		Vector3 direction() const;

		void set_speed(float _speed);
		void set_direction(Vector3 const& _direction);

	private:
		Navigator() = default;
		Navigator(Navigator const& _other);
		Navigator(Navigator&& _other) noexcept;
		Navigator& operator=(Navigator const&) = delete;
		Navigator& operator=(Navigator&&) noexcept = default;

		virtual void _Finalize() override;

		float speed_{};
		bool move_flag_{};
		Vector3 direction_{ -Vector3::UnitY };
		Vector3 destination_{};
		std::list<Vector3> move_path_list_{};
	};
}
