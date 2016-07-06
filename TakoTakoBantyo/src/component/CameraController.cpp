#include "CameraController.h"

#include "input/UtilityGamePad.h"

#include "gameobject/Player.h"
#include "gameobject/Camera.h"

#include "Transform.h"

using namespace ci;
using namespace input;

CameraController::CameraController(GameObject& game_object) :
Component(game_object),
id_(0),
axis_value_(0.0f),
y_axis_value_(gameObject().transform()->getRotate().getAngle())
{
	ci::app::console() << gameObject().transform()->getRotate().getAngle() << std::endl;
}

void CameraController::start()
{
	id_ = static_cast<int>(gameObject().transform()->getParent()->gameObject().getObject<Player>()->getType()) - 1;
	for (auto& child : gameObject().transform()->getChildren())
	{
		camera_ = child->gameObject().getObject<object::Camera>();
	}
}

void CameraController::update()
{
	const auto& gamepad = GamePad::getInstance();

	auto cross = camera_.lock()->getCameraPersp().getViewDirection().cross(Vec3f::yAxis());

	auto axis_value = gamepad.getPressAxis(id_, GamePad::AxisID::RIGHT_Y) * 0.1f;
	y_axis_value_ += gamepad.getPressAxis(id_, GamePad::AxisID::RIGHT_X) * -0.1f;

	axis_value_ += axis_value;

	axis_value_ = std::max(axis_value_, -0.45f);
	axis_value_ = std::min(axis_value_, 0.42f);



	Quatf rotate = Quatf::identity();

	rotate = Quatf(cross, -axis_value_);

	gameObject().transform()->setRotate(Quatf(Vec3f::yAxis(), y_axis_value_) * rotate);
}