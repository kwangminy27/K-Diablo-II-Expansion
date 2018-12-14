#pragma once

namespace K
{
	class TextActor final : public ActorClient
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

		void Move(Vector3 const& _v);

		void set_text(std::wstring const& _text);
		void set_color(Vector4 const& _color);

	private:
		TextActor() = default;
		TextActor(TextActor const& _other);
		TextActor(TextActor&& _other) noexcept;
		TextActor& operator=(TextActor const&) = delete;
		TextActor& operator=(TextActor&&) noexcept = default;

		virtual void _Finalize() override;
	};
}
