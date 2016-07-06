#include "PlayerSunction.h"

#include "input/UtilityGamePad.h"

#include "gameobject/Player.h"

using namespace input;

PlayerSunction::PlayerSunction(GameObject& gameobject):
Component(gameobject),
id_(0)
{

}


void PlayerSunction::start()
{
	player_ = gameObject().getObject<Player>();
	id_ = static_cast<int>(player_.lock()->getType()) - 1;
}

void PlayerSunction::update()
{
	const auto& gamepad = GamePad::getInstance();
	if (gamepad.isPushButton(id_, GamePad::ButtonID::LEFT))
	{

	}
}
