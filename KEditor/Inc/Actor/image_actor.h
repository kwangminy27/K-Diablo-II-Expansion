#pragma once

namespace K
{
	class ImageActor final : public ActorClient
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

		void set_LT(Vector2 const& _LT);
		void set_RB(Vector2 const& _RB);

	private:
		ImageActor() = default;
		ImageActor(ImageActor const& _other);
		ImageActor(ImageActor&& _other) noexcept;
		ImageActor& operator=(ImageActor const&) = delete;
		ImageActor& operator=(ImageActor&&) noexcept = default;

		virtual void _Finalize() override;

		virtual void _Render(float _time) override;

		Vector2 LT_{};
		Vector2 RB_{};
	};
}
