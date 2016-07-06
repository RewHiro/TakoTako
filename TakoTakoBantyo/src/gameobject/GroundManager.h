#pragma once

#include "GameObject.h"

class GroundManager : public GameObject
{

public:

	GroundManager();
	static std::shared_ptr<GroundManager>const create();

private:

	static const int BLOCK_NUM;
};