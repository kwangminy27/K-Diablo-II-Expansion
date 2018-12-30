#pragma once

#include "player_actor.h"

namespace K
{
	class K_ENGINE_DLL Amazon final : public PlayerActor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

	private:
		Amazon() = default;
		Amazon(Amazon const& _other);
		Amazon(Amazon&& _other) noexcept;
		Amazon& operator=(Amazon const&) = delete;
		Amazon& operator=(Amazon&&) noexcept = default;

		virtual void _Finalize() override;
	};
}
