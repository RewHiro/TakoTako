#pragma once

#include "GameObject.h"

class CrabArmManager : public GameObject
{
public:
	CrabArmManager();
	static std::shared_ptr<CrabArmManager>const create();
};