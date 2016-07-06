#pragma once

#include "GameObject.h"

class PlayerShot;
class Player;

class UIBullet : public GameObject
{

public:

	UIBullet(const std::shared_ptr<Player>& player);
	static std::shared_ptr<UIBullet>const create(const std::shared_ptr<Player>& player);

private:

	void update()final;
	void resize()final;
	void draw()final;

	int count_;
	std::weak_ptr<PlayerShot>player_shot_;
	ci::Font font_;
};