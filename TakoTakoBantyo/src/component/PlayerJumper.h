#pragma once

#include "Component.h"

class Player;

class PlayerJumper : public Component
{
public:

	PlayerJumper(GameObject& gameobject);

	void changeCanJump()
	{ 
		jump_power_ = 0.0f;
		is_jump_ = false;
	}

	bool isJump()const{ return is_jump_; }

private:

	static const float JUMP_POWER;
	static const float GRAVITY_POWER;
	static const float LIMIT_POS_Y;

	void start()final;
	void update()final;

	void onCollisionEnter(GameObject& gameobject)final;
	void onCollisionExit(GameObject& gameobject)final;
	void momentCollisionExit(GameObject& gameobject)final;

	void jump();
	void gravity();

	int id_;
	std::weak_ptr<Player>player_;
	float jump_power_;
	bool is_jump_;
	bool is_rock_hit_;
};