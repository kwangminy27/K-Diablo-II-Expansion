#include "KEngine.h"
#include "npc_actor.h"

K::NPCActor::NPCActor(NPCActor const& _other) : Actor(_other)
{
}

K::NPCActor::NPCActor(NPCActor&& _other) noexcept : Actor(std::move(_other))
{
}
