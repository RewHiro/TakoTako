#pragma once

#include "GameObject.h"

class Player;

class UIManager : public GameObject
{

public:

	UIManager(const std::shared_ptr<Player>& player);
	static std::shared_ptr<UIManager>const create(const std::shared_ptr<Player>& player);

private:

	void update()final;

	std::weak_ptr<Player>player_;
	bool is_create_resutl_;
};