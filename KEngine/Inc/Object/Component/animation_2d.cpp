#include "KEngine.h"
#include "animation_2d.h"

#include "Resource/resource_manager.h"
#include "Rendering/rendering_manager.h"
#include "Object/Actor/actor.h"
#include "Object/Component/transform.h"
#include "Object/Component/material.h"

std::shared_ptr<K::ANIMATION_2D_CLIP_DESC> K::Animation2D::animation_2d_clip_dummy_{};

void K::Animation2D::Initialize()
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
		std::cout << "Animation2D::Initialize" << std::endl;
	}
}

void K::Animation2D::Update(float _time)
{
	elapsed_time_ += _time;

	float interval = current_clip_->completion_time / current_clip_->frame_vector.size();

	while (elapsed_time_ >= interval)
	{
		elapsed_time_ -= interval;

		++frame_idx_;

		if (frame_idx_ == current_clip_->frame_vector.size())
		{
			frame_idx_ = 0;

			switch (current_clip_->option)
			{
			case ANIMATION_OPTION::DESTROY:
				owner()->set_tag_state(TAG_STATE::DEAD);
				break;
			}
		}
	}
}

K::CPTR K::Animation2D::Clone() const
{
	return CPTR{ new Animation2D{ *this }, [](Component* _p) {
		static_cast<Animation2D*>(_p)->_Finalize();
		delete static_cast<Animation2D*>(_p);
	} };
}

void K::Animation2D::Serialize(InputMemoryStream& _imstream)
{
}

void K::Animation2D::Serialize(OutputMemoryStream& _omstream)
{
}

void K::Animation2D::UpdateConstantBuffer(float _time)
{
	Animation2DConstantBuffer animation_2d_CB{};

	switch (current_clip_->type)
	{
	case ANIMATION_2D_TYPE::FRAME:
		animation_2d_CB.LT = { 0.f, 0.f };
		animation_2d_CB.RB = { 1.f, 1.f };
		break;

	case ANIMATION_2D_TYPE::ATLAS:
		animation_2d_CB.LT.x = current_clip_->frame_vector.at(frame_idx_).LT.x / current_clip_->width;
		animation_2d_CB.LT.y = current_clip_->frame_vector.at(frame_idx_).LT.y / current_clip_->height;
		animation_2d_CB.RB.x = current_clip_->frame_vector.at(frame_idx_).RB.x / current_clip_->width;
		animation_2d_CB.RB.y = current_clip_->frame_vector.at(frame_idx_).RB.y / current_clip_->height;
		break;
	}

	RenderingManager::singleton()->UpdateConstantBuffer(ANIMATION_2D, &animation_2d_CB);
}

std::shared_ptr<K::ANIMATION_2D_CLIP_DESC> const& K::Animation2D::FindClip(std::string const& _tag)
{
	auto iter = animation_2d_clip_map_.find(_tag);

	if (iter == animation_2d_clip_map_.end())
		return animation_2d_clip_dummy_;

	return iter->second;
}

void K::Animation2D::AddClip(std::string const& _tag)
{
	if (FindClip(_tag))
		throw std::exception{ "Animation2D::AddClip" };

	auto const& animation_2d_clip = ResourceManager::singleton()->FindAnimation2DClip(_tag);

	if (nullptr == animation_2d_clip)
		throw std::exception{ "Animation2D::AddClip" };

	animation_2d_clip_map_.insert(std::make_pair(_tag, animation_2d_clip));
}

void K::Animation2D::SetCurrentClip(std::string const& _tag)
{
	auto clip = FindClip(_tag);

	if(nullptr == clip)
		throw std::exception{ "Animation2D::SetCurrentClip" };

	current_clip_ = clip;

	frame_idx_ = 0;
	elapsed_time_ = 0.f;

	Vector3 frame_size = Vector3{ current_clip_->frame_vector.at(0).RB.x, current_clip_->frame_vector.at(0).RB.y, 1.f };

	CPTR_CAST<Transform>(owner()->FindComponent(TAG{ TRANSFORM, 0 }))->set_local_scaling(frame_size);
	CPTR_CAST<Material>(owner()->FindComponent(TAG{ MATERIAL, 0 }))->SetTexture(current_clip_->texture_tag, 0, 0, 0);
}

void K::Animation2D::SetDefaultClip(std::string const& _tag)
{
	auto clip = FindClip(_tag);

	if (nullptr == clip)
		throw std::exception{ "Animation2D::SetDefaultClip" };

	default_clip_ = clip;
}

K::Animation2D::Animation2D(Animation2D const& _other) : Component(_other)
{
	elapsed_time_ = _other.elapsed_time_;
	frame_idx_ = _other.frame_idx_;
	current_clip_ = _other.current_clip_;
	default_clip_ = _other.default_clip_;
	animation_2d_clip_map_ = _other.animation_2d_clip_map_;
}

K::Animation2D::Animation2D(Animation2D&& _other) noexcept : Component(std::move(_other))
{
	elapsed_time_ = std::move(_other.elapsed_time_);
	frame_idx_ = std::move(_other.frame_idx_);
	current_clip_ = std::move(_other.current_clip_);
	default_clip_ = std::move(_other.default_clip_);
	animation_2d_clip_map_ = std::move(_other.animation_2d_clip_map_);
}

void K::Animation2D::_Finalize()
{
}
