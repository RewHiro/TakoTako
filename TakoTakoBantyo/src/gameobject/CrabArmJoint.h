#pragma once

#include "GameObject.h"

class Item;

class CrabArmJoint : public GameObject
{
public:
	CrabArmJoint(const ci::Vec3f& pos, int num);
	static std::shared_ptr<CrabArmJoint>const create(const ci::Vec3f& pos, int num);


private:

	void start()final;
	void update()final;

	void inMove();

	std::weak_ptr<Item>item_;
	float count_;
	int num_;
};