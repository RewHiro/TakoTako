#pragma once

#include "Component.h"

class Player;

class PlayerSunction : public Component
{

public:

	PlayerSunction(GameObject& gameobject);


private:

	void start()final;
	void update()final;

	int id_;
	std::weak_ptr<Player>player_;

};