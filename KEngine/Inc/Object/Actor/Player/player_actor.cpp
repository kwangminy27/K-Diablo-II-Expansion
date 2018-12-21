#include "KEngine.h"
#include "player_actor.h"

K::PlayerActor::PlayerActor(PlayerActor const& _other) : Actor(_other)
{
}

K::PlayerActor::PlayerActor(PlayerActor && _other) noexcept : Actor(std::move(_other))
{
}
