#include "KEngine.h"
#include "player_actor.h"

void K::PlayerActor::set_focus_flag(bool _flag)
{
	focus_flag_ = _flag;
}

K::PlayerActor::PlayerActor(PlayerActor const& _other) : Actor(_other)
{
	focus_flag_ = _other.focus_flag_;
}

K::PlayerActor::PlayerActor(PlayerActor&& _other) noexcept : Actor(std::move(_other))
{
	focus_flag_ = std::move(_other.focus_flag_);
}
