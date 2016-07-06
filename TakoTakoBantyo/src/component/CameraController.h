#pragma once

#include "Component.h"

namespace object
{
	class Camera;
}

class CameraController : public Component
{
public:

	CameraController(GameObject& game_object);

private:

	void start()final;
	void update()final;

	int id_;
	std::weak_ptr<object::Camera>camera_;
	float axis_value_;
	float y_axis_value_;
};