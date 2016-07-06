#include "SeaWeedManager.h"

#include "cinder/Rand.h"

#include "GameObjectTask.h"
#include "SeaWeed.h"

#include "component/Transform.h"

using namespace ci;

const int SeaWeedManager::SEA_WEED_NUM = 50;

SeaWeedManager::SeaWeedManager(bool is_title) :
GameObject("SeaWeedManager"),
is_title_(is_title)
{
	float scale = 10;
	float area = 60;
	for (auto i = 0; i < SEA_WEED_NUM; ++i)
	{
		auto sea_weed = SeaWeed::create(Vec3f(Rand::randFloat(-area, area), 4, Rand::randFloat(-area, area)), Vec3f(scale, scale, 1),is_title_);
		sea_weed->transform()->setParent(transform_);
	}
}

std::shared_ptr<SeaWeedManager>const SeaWeedManager::create(bool is_title)
{
	auto instance = std::make_shared<SeaWeedManager>(is_title);
	GameObjectTask::getInstnace().add(instance);
	return instance;
}