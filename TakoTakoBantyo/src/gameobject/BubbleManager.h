#pragma once

#include "GameObject.h"

class BubbleManager : public GameObject
{
public:
	BubbleManager(const ci::Vec3f& pos);
	static std::shared_ptr<BubbleManager>const create(const ci::Vec3f& pos);

private:

	const float SPEED;
	const float SWING;

	void update()final;

	ci::Vec3f center_point_;
	int create_count_;
};