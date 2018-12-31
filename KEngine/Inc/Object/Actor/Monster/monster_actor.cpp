#include "KEngine.h"
#include "monster_actor.h"

float K::MonsterActor::hp() const
{
	return hp_;
}

float K::MonsterActor::speed() const
{
	return speed_;
}

K::Vector3 const& K::MonsterActor::direction() const
{
	return direction_;
}

K::APTR K::MonsterActor::target() const
{
	return target_.lock();
}

void K::MonsterActor::AddHp(float _value)
{
	hp_ += _value;
}

void K::MonsterActor::set_target(APTR const& _actor)
{
	target_ = _actor;
}

void K::MonsterActor::set_direction(Vector3 const& _direction)
{
	direction_ = _direction;
}

void K::MonsterActor::set_hp(float _hp)
{
	hp_ = _hp;
}

void K::MonsterActor::set_speed(float _speed)
{
	speed_ = _speed;
}

K::MonsterActor::MonsterActor(MonsterActor const& _other) : Actor(_other)
{
	speed_ = _other.speed_;
	direction_ = _other.direction_;
	target_ = _other.target_;
}

K::MonsterActor::MonsterActor(MonsterActor&& _other) noexcept : Actor(std::move(_other))
{
	speed_ = std::move(_other.speed_);
	direction_ = std::move(_other.direction_);
	target_ = std::move(_other.target_);
}
