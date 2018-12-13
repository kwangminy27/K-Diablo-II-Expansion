#pragma once

#include "actor.h"

namespace K
{
	class K_ENGINE_DLL TileActor final : public Actor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

		TILE_OPTION option() const;

		void set_texture_tag(std::string const& _tag);
		void set_option(TILE_OPTION _option);

	private:
		TileActor() = default;
		TileActor(TileActor const& _other);
		TileActor(TileActor&& _other) noexcept;
		TileActor& operator=(TileActor const&) = delete;
		TileActor& operator=(TileActor&&) noexcept = default;

		virtual void _Finalize() override;

		TILE_OPTION option_{};
		std::string texture_tag_{};
	};
}
