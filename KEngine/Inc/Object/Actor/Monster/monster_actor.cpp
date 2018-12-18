#include "KEngine.h"
#include "monster_actor.h"

K::MonsterActor::MonsterActor(MonsterActor const& _other) : Actor(_other)
{
}

K::MonsterActor::MonsterActor(MonsterActor&& _other) noexcept : Actor(std::move(_other))
{
}
