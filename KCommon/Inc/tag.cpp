#include "KCommon.h"
#include "tag.h"

K::TAG_STATE K::Tag::tag_state() const
{
	return tag_state_;
}

K::TAG const& K::Tag::tag() const
{
	return tag_;
}

void K::Tag::set_tag_state(TAG_STATE _state)
{
	tag_state_ = _state;
}

void K::Tag::set_tag(TAG const& _tag)
{
	tag_ = _tag;
}

K::Tag::Tag(Tag const& _other)
{
	*this = _other;
}

K::Tag::Tag(Tag&& _other) noexcept
{
	*this = std::move(_other);
}
