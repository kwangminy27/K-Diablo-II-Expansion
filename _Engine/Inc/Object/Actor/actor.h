#pragma once

namespace K
{
	class Level;
	class Layer;

	enum class ACTOR_STATE
	{
		ATTACK1,
		ATTACK2,
		GET_HIT,
		DEAD,
		DEATH,
		NEUTRAL,
		RUN,
		SPECIAL_CAST,
		SPECIAL1,
		WALK,
		MAX
	};

	enum class ELEMENT_STATE
	{
		NORMAL,
		COLD,
		FIRE,
		POISON
	};

	class K_ENGINE_DLL Actor : public Tag, public std::enable_shared_from_this<Actor>
	{
		friend class Layer;
		friend class ObjectManager;
	public:
		virtual void Initialize() = 0;

		virtual APTR Clone() const = 0;

		virtual void Serialize(InputMemoryStream& _imstream) = 0;
		virtual void Serialize(OutputMemoryStream& _omstream) = 0;

		CPTR const& FindComponent(TAG const& _tag) const;
		std::list<Component*> const& FindComponent(std::string const& _tag) const;
		APTR const& FindChild(TAG const& _tag) const;

		void AddComponent(CPTR& _component);
		void AddChild(APTR& _child);

		void RemoveComponent(CPTR const& _component);
		void RemoveChild(APTR const& _child);

		int shadow_dir_idx() const;

		bool ui_flag() const;
		ACTOR_STATE state() const;
		ELEMENT_STATE element_state() const;
		std::shared_ptr<Level> level() const;
		std::shared_ptr<Layer> layer() const;
		APTR parent() const;
		std::list<CPTR> const& component_list() const;
		std::list<APTR> const& child_list() const;

		void set_ui_flag(bool _flag);
		void set_state(ACTOR_STATE _state);
		void set_element_state(ELEMENT_STATE _state);
		void set_parent(APTR const& _actor);
		void set_level(std::shared_ptr<Level> const& _level);
		void set_layer(std::shared_ptr<Layer> const& _layer);

		static CPTR component_dummy_;
		static APTR child_dummy_;

	protected:
		Actor() = default;
		Actor(Actor const& _other);
		Actor(Actor&& _other) noexcept;
		Actor& operator=(Actor const&) = delete;
		Actor& operator=(Actor&&) noexcept = default;
		virtual ~Actor() = default;

		virtual void _Finalize() = 0;

		virtual void _Input(float _time);
		virtual void _Update(float _time);
		virtual void _Render(float _time);

		void __Input(float _time);
		void __Update(float _time);
		void __Render(float _time);

		int shadow_dir_idx_{};
		float element_time_{};

		bool ui_flag_{};
		ACTOR_STATE state_{};
		ELEMENT_STATE element_state_{};
		std::weak_ptr<Level> level_{};
		std::weak_ptr<Layer> layer_{};
		std::weak_ptr<Actor> parent_{};
		std::list<CPTR> component_list_{};
		std::list<APTR> child_list_{};
	};

	class K_ENGINE_DLL ActorClient : public Actor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() = 0;

		virtual APTR Clone() const = 0;

		virtual void Serialize(InputMemoryStream& _imstream) = 0;
		virtual void Serialize(OutputMemoryStream& _omstream) = 0;

	protected:
		ActorClient() = default;
		ActorClient(ActorClient const& _other);
		ActorClient(ActorClient&& _other) noexcept;
		ActorClient& operator=(ActorClient const&) = delete;
		ActorClient& operator=(ActorClient&&) noexcept = default;

		virtual void _Finalize() = 0;
	};

	class K_ENGINE_DLL ActorServer : public Actor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() = 0;

		virtual APTR Clone() const = 0;

		virtual void Serialize(InputMemoryStream& _imstream) = 0;
		virtual void Serialize(OutputMemoryStream& _omstream) = 0;

	protected:
		ActorServer() = default;
		ActorServer(ActorServer const& _other);
		ActorServer(ActorServer&& _other) noexcept;
		ActorServer& operator=(ActorServer const&) = delete;
		ActorServer& operator=(ActorServer&&) noexcept = default;

		virtual void _Finalize() = 0;
	};
}
