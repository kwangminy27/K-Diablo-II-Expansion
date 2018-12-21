#pragma once

#include "npc_actor.h"

namespace K
{
	class K_ENGINE_DLL Akara final : public NPCActor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

	private:
		Akara() = default;
		Akara(Akara const& _other);
		Akara(Akara&& _other) noexcept;
		Akara& operator=(Akara const&) = delete;
		Akara& operator=(Akara&&) noexcept = default;

		virtual void _Finalize() override;
	};
}
