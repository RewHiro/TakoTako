#pragma once

#include "GameObject.h"

class PlayerArmManager : public GameObject
{
public:

	PlayerArmManager();
	static std::shared_ptr<PlayerArmManager>const create();
};