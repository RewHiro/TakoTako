#pragma once

#include "GameObject.h"

class Item;

class CrabFootJoint : public GameObject
{
public:

	CrabFootJoint(const ci::Vec3f& pos, int num);
	static std::shared_ptr<CrabFootJoint>const create(const ci::Vec3f& pos, int num);

private:

	float DIFF_THETA;

	void start()final;
	void update()final;

	void inMove();

	std::weak_ptr<Item>item_;
	float move_count_;
	int num_;
};