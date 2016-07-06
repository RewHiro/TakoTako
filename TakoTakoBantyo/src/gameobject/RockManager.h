#pragma once

#include "GameObject.h"

class RockManager : public GameObject
{

public:
	RockManager();
	static  std::shared_ptr<RockManager>const create();

private:
	
	void update()final;

	static const int ROCK_NUM;

	int create_count_;
};