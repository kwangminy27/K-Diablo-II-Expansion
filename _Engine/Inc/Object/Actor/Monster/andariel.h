#pragma once

#include "monster_actor.h"

namespace K
{
	class K_ENGINE_DLL Andariel final : public MonsterActor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

	private:
		Andariel() = default;
		Andariel(Andariel const& _other);
		Andariel(Andariel&& _other) noexcept;
		Andariel& operator=(Andariel const&) = delete;
		Andariel& operator=(Andariel&&) noexcept = default;

		virtual void _Finalize() override;

		virtual void _Input(float _time) override;
		virtual void _Update(float _time) override;

		bool death_flag_{};
	};
}
