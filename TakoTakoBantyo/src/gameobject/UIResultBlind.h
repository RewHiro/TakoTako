#pragma once

#include "GameObject.h"

class Player;

class UIResultBlind : public GameObject
{
public:
	UIResultBlind();
	static std::shared_ptr<UIResultBlind>const create(std::shared_ptr<Player>const player);

private:

	void draw()final;
};