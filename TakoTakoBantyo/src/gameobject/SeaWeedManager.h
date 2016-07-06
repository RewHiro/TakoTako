#pragma once

#include "GameObject.h"

class SeaWeedManager : public GameObject
{
public:

	explicit SeaWeedManager(bool is_title = false);
	static std::shared_ptr<SeaWeedManager>const create(bool is_title = false);

private:

	static const int SEA_WEED_NUM;
	bool is_title_;
};