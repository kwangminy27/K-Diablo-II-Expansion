#pragma once

#include "monster_actor.h"

namespace K
{
	class K_ENGINE_DLL Wendigo final : public MonsterActor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

	private:
		Wendigo() = default;
		Wendigo(Wendigo const& _other);
		Wendigo(Wendigo&& _other) noexcept;
		Wendigo& operator=(Wendigo const&) = delete;
		Wendigo& operator=(Wendigo&&) noexcept = default;

		virtual void _Finalize() override;

		virtual void _Input(float _time) override;
		virtual void _Update(float _time) override;
	};
}
