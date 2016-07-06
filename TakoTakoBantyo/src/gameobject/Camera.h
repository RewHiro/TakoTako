#pragma once

#include "GameObject.h"

#include "cinder/Camera.h"
#include "cinder/Area.h"

namespace object
{
	class Camera : public GameObject
	{
	public:
		
		Camera(const std::string& name);

		static GameObjectSP create(int num);

		ci::CameraPersp& getCameraPersp(){ return camera_persp_; }
		ci::CameraOrtho& getCameraOrtho(){ return camera_ortho_; }
		ci::Area getViewPortArea()const { return view_port_area_; }
		void setViewPortArea(const ci::Area& area){ view_port_area_ = area; }

		void start();
		void update();
		void resize();

	private:

		ci::CameraPersp camera_persp_;
		ci::CameraOrtho camera_ortho_;
		ci::Area view_port_area_;

		GameObject const* player_;
	};
}