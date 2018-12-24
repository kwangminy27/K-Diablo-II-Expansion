#include "KEngine.h"
#include "navigator.h"

#include "World/world_manager.h"
#include "Object/Actor/tile_map_actor.h"
#include "Object/Component/transform.h"
#include "navigation_manager.h"

void K::Navigator::Initialize()
{
	try
	{
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Navigator::Initialize" << std::endl;
	}
}

void K::Navigator::Update(float _time)
{
	if (false == move_flag_)
		return;

	auto const& transform = CPTR_CAST<Transform>(owner()->FindComponent(TAG{ TRANSFORM, 0 }));

	auto position = transform->world().Translation();

	direction_ = destination_ - position;
	direction_.Normalize();

	auto displacement = transform->local_translation() + direction_ * speed_ * _time;
	transform->set_local_translation(displacement);

	if (Vector3::Distance(displacement, destination_) < 8.f)
	{
		if (false == move_path_list_.empty())
		{
			destination_ = move_path_list_.front();
			move_path_list_.pop_front();
		}
		else
		{
			owner()->set_state(ACTOR_STATE::NEUTRAL);

			move_flag_ = false;
		}
	}
}

K::CPTR K::Navigator::Clone() const
{
	return CPTR{ new Navigator{ *this }, [](Component* _p) {
		static_cast<Navigator*>(_p)->_Finalize();
		delete static_cast<Navigator*>(_p);
	} };
}

void K::Navigator::Serialize(InputMemoryStream& _imstream)
{
}

void K::Navigator::Serialize(OutputMemoryStream& _omstream)
{
}

void K::Navigator::Route(Vector3 const& _start, Vector3 const& _end)
{
	auto const& navigation_manager = NavigationManager::singleton();

	if (navigation_manager->Route(_start, _end))
	{
		move_path_list_ = navigation_manager->move_path_list();

		destination_ = move_path_list_.front();
		move_path_list_.pop_front();

		move_flag_ = true;
	}
	else
		owner()->set_state(ACTOR_STATE::NEUTRAL);
}

bool K::Navigator::move_flag() const
{
	return move_flag_;
}

K::Vector3 K::Navigator::direction() const
{
	return direction_;
}

void K::Navigator::set_speed(float _speed)
{
	speed_ = _speed;
}

K::Navigator::Navigator(Navigator const& _other) : Component(_other)
{
	speed_ = _other.speed_;
	move_flag_ = _other.move_flag_;
	direction_ = _other.direction_;
	destination_ = _other.destination_;
	move_path_list_ = _other.move_path_list_;
}

K::Navigator::Navigator(Navigator&& _other) noexcept : Component(std::move(_other))
{
	speed_ = std::move(_other.speed_);
	move_flag_ = std::move(_other.move_flag_);
	direction_ = std::move(_other.direction_);
	destination_ = std::move(_other.destination_);
	move_path_list_ = std::move(_other.move_path_list_);
}

void K::Navigator::_Finalize()
{
}
