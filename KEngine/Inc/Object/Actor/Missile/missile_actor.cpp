#include "KEngine.h"
#include "missile_actor.h"

K::MissileActor::MissileActor(MissileActor const& _other) : Actor(_other)
{
}

K::MissileActor::MissileActor(MissileActor&& _other) noexcept : Actor(std::move(_other))
{
}
