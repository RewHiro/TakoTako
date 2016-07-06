#pragma once

#include "Component.h"

class Player;

class PlayerShot : public Component
{
public :

	PlayerShot(GameObject& gameobject);

	int getBulletCount()const{ return bullet_count_; }
	void addBulletCount(int bullet_count){ bullet_count_ += bullet_count; }

private:

	static const int MAX_BULLE_NUM;

	void start()final;
	void update()final;

	void onCollisionEnter(GameObject& gameobject)final;
	void onCollisionExit(GameObject& gameobject)final;

	void shot();
	void countLimit();

	float shot_count_;
	int id_;
	int bullet_count_;
	std::weak_ptr<Player>player_;
	ci::audio::VoiceRef se_;
};