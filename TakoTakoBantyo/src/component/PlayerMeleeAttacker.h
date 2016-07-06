#pragma once

#include "Component.h"

class Player;

class PlayerMeleeAttacker : public Component
{
public:

	PlayerMeleeAttacker(GameObject& gameobject);

private:

	void start()final;
	void update()final;

	void onCollisionEnter(GameObject& gameobject)final;
	void onCollisionExit(GameObject& gameobject)final;

	void attackAction();
	void inAttack();

	int id_;
	ci::Vec3f speed_;
	std::weak_ptr<Player>player_;
};