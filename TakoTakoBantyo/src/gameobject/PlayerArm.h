#pragma once

#include "GameObject.h"

class PlayerArm : public GameObject
{
public:

	PlayerArm();

private:

	enum class Type
	{
		LEFT,
		RIGHT,
	};

	Type type_;

};