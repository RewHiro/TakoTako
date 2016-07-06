#pragma once

#include "GameObject.h"

class Player;

class PlayerArmJoint : public GameObject
{
public:

	enum class Type
	{
		LEFT,
		RIGHT,
	};

	PlayerArmJoint(const ci::Vec3f& pos, const Type type);
	static std::shared_ptr<PlayerArmJoint>const create(const ci::Vec3f& pos, const Type type);

private:

	void start()final;
	void update()final;

	void notAttackAnimation();
	void waitAnimation();
	void moveAnimation();
	void meleeAttackAnimation();

	std::weak_ptr<Player>player_;
	float angle_;
	float theta_;
	Type type_;
};