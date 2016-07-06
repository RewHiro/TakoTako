#pragma once

#include "GameObject.h"

class Player;

class UIHitPointManager : public GameObject
{
public:

	explicit UIHitPointManager(const std::shared_ptr<Player>& player);
	static std::shared_ptr<UIHitPointManager>const create(const std::shared_ptr<Player>& player);

private:

	void start()final;
	void update()final;

	void increaseHP(int player_hp);
	void reductionHP(int player_hp);

	int player_hp_;
	std::weak_ptr<Player>player_;
};