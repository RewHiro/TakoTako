#pragma once
#include "Component.h"

#include <memory>

class Player;

class PlayerMover : public Component
{
public:

	PlayerMover(GameObject& gameobject);

	ci::Vec3f getDirection()const{ return direction_; }
	ci::Vec3f getMovingDistance()const{ return moving_distance_; }

private:

	void start()final;
	void update()final;

	void onCollisionEnter(GameObject& gameobject)final;
	void onCollisionExit(GameObject& gameobject)final;

	std::weak_ptr<Player> player_;
	GameObject* camera_manager_;
	std::weak_ptr<object::Camera>camera_;
	ci::Vec3f direction_;
	ci::Vec3f moving_distance_;
	int id_;
};