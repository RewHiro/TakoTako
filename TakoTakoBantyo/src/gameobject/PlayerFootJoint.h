#pragma once

#include "GameObject.h"

class Player;

class PlayerFootJoint : public GameObject
{
public:

	PlayerFootJoint(const ci::Vec3f& pos, float theta);
	static std::shared_ptr<PlayerFootJoint>const create(const ci::Vec3f& pos, float theta);

	void startDrained(const ci::Vec3f& drain_pos);

private:

	const float DIFF_THETA;

	void start()final;
	void update()final;

	void waitAnimation();
	void moveAnimation();
	void meleeAttackAnimation();
	void drawAnimmation();

	std::weak_ptr<Player>player_;
	float theta_;
	float angle_;
	float drained_count_;
	bool is_wait_;
	bool is_drained_;
};