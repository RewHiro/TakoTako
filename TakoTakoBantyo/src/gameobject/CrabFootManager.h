#pragma once

#include "GameObject.h"

class CrabFootManager : public GameObject
{
public:
	CrabFootManager();
	static std::shared_ptr<CrabFootManager>const create();
};