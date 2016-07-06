#pragma once

#include "Component.h"

class Player;

class PlayerDrainer : public Component
{
public:

	PlayerDrainer(GameObject& gameobject);

private:

	void start()final;
	void update()final;

	void drainAction();

	int id_;
	float count_;
	std::weak_ptr<Player>player_;
	ci::audio::VoiceRef se_;
};