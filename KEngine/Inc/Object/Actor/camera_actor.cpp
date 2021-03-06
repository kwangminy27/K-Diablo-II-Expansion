#include "KEngine.h"
#include "camera_actor.h"

#include "Object/object_manager.h"
#include "Object/Component/transform.h"
#include "Object/Component/camera.h"

void K::CameraActor::Initialize()
{
	try
	{
		auto const& object_manager = ObjectManager::singleton();

		auto transform = object_manager->CreateComponent<Transform>(TAG{ TRANSFORM, 0 });
		AddComponent(transform);

		auto camera = object_manager->CreateComponent<Camera>(TAG{ CAMERA, 0 });
		AddComponent(camera);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "CameraActor::Initialize" << std::endl;
	}
}

K::APTR K::CameraActor::Clone() const
{
	return APTR{ new CameraActor{ *this }, [](CameraActor* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::CameraActor::Serialize(InputMemoryStream& _imstream)
{
}

void K::CameraActor::Serialize(OutputMemoryStream& _omstream)
{
}

void K::CameraActor::CreateView(Vector3 const& _eye, Vector3 const& _focus, Vector3 const& _up)
{
	CPTR_CAST<Camera>(FindComponent(TAG{ CAMERA, 0 }))->CreateView(_eye, _focus, _up);
}

void K::CameraActor::CreateProjection(float _width, float _height, float _near, float _far)
{
	CPTR_CAST<Camera>(FindComponent(TAG{ CAMERA, 0 }))->CreateProjection(_width, _height, _near, _far);
}

void K::CameraActor::CreateProjection(float _fov_angle, float _width, float _height, float _near, float _far)
{
	CPTR_CAST<Camera>(FindComponent(TAG{ CAMERA, 0 }))->CreateProjection(_fov_angle, _width, _height, _near, _far);
}

float K::CameraActor::scale() const
{
	return scale_;
}

K::Vector2 const& K::CameraActor::resolution() const
{
	return CPTR_CAST<Camera>(FindComponent(TAG{ CAMERA, 0 }))->resolution();
}

K::Matrix const& K::CameraActor::view() const
{
	return CPTR_CAST<Camera>(FindComponent(TAG{ CAMERA, 0 }))->view();
}

K::Matrix const& K::CameraActor::projection() const
{
	return CPTR_CAST<Camera>(FindComponent(TAG{ CAMERA, 0 }))->projection();
}

void K::CameraActor::set_scale(float _scale)
{
	scale_ = _scale;
}

K::CameraActor::CameraActor(CameraActor const& _other) : Actor(_other)
{
	scale_ = _other.scale_;
}

K::CameraActor::CameraActor(CameraActor&& _other) noexcept : Actor(std::move(_other))
{
	scale_ = std::move(_other.scale_);
}

void K::CameraActor::_Finalize()
{
}

void K::CameraActor::_Update(float _time)
{
}
