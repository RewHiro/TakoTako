#pragma once

#include "GameObject.h"

class PlayerFootManager : public GameObject
{
public:

	PlayerFootManager();
	static std::shared_ptr<PlayerFootManager>const create();
};