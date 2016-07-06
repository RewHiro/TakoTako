#include "Camera.h"

#include "cinder/app/App.h"

#include "component/Transform.h"

#include "GameObjectTask.h"

using namespace ci::app;

namespace object
{

	GameObjectSP Camera::create(int num)
	{
		auto camera = std::make_shared<Camera>("Camera" + std::to_string(num));
		GameObjectTask::getInstnace().addCamera(camera);
		return camera;
	}

	void Camera::start()
	{
		player_ = &(transform()->getTopOfLevelParent()->gameObject());

		auto tranlsate = ci::Matrix44f::createTranslation(player_->transform()->getPos());
		auto rotate = transform()->getParent()->getRotate().toMatrix44();
		auto camera_translate = ci::Matrix44f::createTranslation(transform()->getPos());

		auto affine = tranlsate * rotate * camera_translate;

		auto target = ci::Vec3f(affine.m03, affine.m13, affine.m23);

		camera_persp_.setEyePoint(target);
		camera_persp_.setCenterOfInterestPoint(player_->transform()->getPos());
	}

	void Camera::update()
	{
		auto tranlsate = ci::Matrix44f::createTranslation(player_->transform()->getPos());
		auto rotate = transform()->getParent()->getRotate().toMatrix44();
		auto camera_translate = ci::Matrix44f::createTranslation(transform()->getPos());

		auto affine = tranlsate * rotate * camera_translate;

		auto target = ci::Vec3f(affine.m03, affine.m13, affine.m23);

		auto current_pos = camera_persp_.getEyePoint();

		auto diff = (target - current_pos) * 0.2f;

		camera_persp_.setEyePoint(current_pos + diff);
		camera_persp_.setCenterOfInterestPoint(player_->transform()->getPos());
	}

	void Camera::resize()
	{
		auto width = getWindowWidth()*0.5f;
		auto height = getWindowHeight()*0.5f;
		camera_ortho_.setOrtho(-width, width, -height, height, 0.5f, 1000.0f);
		camera_ortho_.lookAt(ci::Vec3f::zAxis() * 10, ci::Vec3f::zero(), ci::Vec3f::yAxis());

		camera_persp_.setPerspective(camera_persp_.getFov(), getWindowAspectRatio(), camera_persp_.getNearClip(), camera_persp_.getFarClip());
	}

	Camera::Camera(const std::string& name) :
		GameObject(name),
		view_port_area_(ci::Area(0, 0, getWindowWidth(), getWindowHeight()))
	{
		camera_persp_.setPerspective(90, getWindowAspectRatio(), 0.5f, 1000.0f);
		camera_persp_.lookAt(ci::Vec3f::zAxis() * -10, ci::Vec3f::zero(), ci::Vec3f::yAxis());
		transform()->setPos(ci::Vec3f(0, 11, -15));

		//transform()->setPos(ci::Vec3f(0, 5, -5));
		resize();
	}
}