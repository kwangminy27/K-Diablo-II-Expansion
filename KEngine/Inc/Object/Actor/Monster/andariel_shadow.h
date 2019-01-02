#pragma once

#include "monster_actor.h"

namespace K
{
	class K_ENGINE_DLL AndarielShadow final : public MonsterActor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

	private:
		AndarielShadow() = default;
		AndarielShadow(AndarielShadow const& _other);
		AndarielShadow(AndarielShadow&& _other) noexcept;
		AndarielShadow& operator=(AndarielShadow const&) = delete;
		AndarielShadow& operator=(AndarielShadow&&) noexcept = default;

		virtual void _Finalize() override;

		virtual void _Render(float _time) override;
	};
}
