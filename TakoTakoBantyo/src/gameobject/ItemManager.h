#pragma once

#include <random>

#include "GameObject.h"

class ItemManager : public GameObject
{
public:
	ItemManager();
	static std::shared_ptr<ItemManager>const create();
private:

	void update()final;

	void setCreate();

	std::mt19937 random_;
	int create_count_;
};