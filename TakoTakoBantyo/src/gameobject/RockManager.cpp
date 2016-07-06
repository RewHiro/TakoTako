#include "RockManager.h"

#include "cinder/Rand.h"

#include "GameObjectTask.h"
#include "Rock.h"
#include "BubbleManager.h"

#include "component/Transform.h"

const int RockManager::ROCK_NUM = 10;

using namespace ci;

RockManager::RockManager() :
GameObject("RockManager"),
create_count_(0)
{
	const float pos = 65.0f;

	for (auto i = 0; i < 14; ++i)
	{
		auto rock = Rock::create(
			Vec3f(Rand::randFloat(-pos, pos), 0, Rand::randFloat(-pos, pos)),
			Vec3f(Rand::randFloat(1.0f, 2.0f), Rand::randFloat(1.0f, 2.0f), Rand::randFloat(1.0f, 2.0f)),
			Quatf(Vec3f::yAxis(),Rand::randFloat(0.0f,3.14f)), 
			RockType::ROCK01);
		rock->transform()->setParent(transform_);
	}

	for (auto i = 0; i < 8; ++i)
	{
		auto rock = Rock::create(
			Vec3f(Rand::randFloat(-pos, pos), 0, Rand::randFloat(-pos, pos)),
			Vec3f(Rand::randFloat(1.0f, 2.0f), Rand::randFloat(1.0f, 2.0f), Rand::randFloat(1.0f, 2.0f)),
			Quatf(Vec3f::yAxis(), Rand::randFloat(0.0f, 3.14f)), 
			RockType::ROCK02);
		rock->transform()->setParent(transform_);
	}
}

std::shared_ptr<RockManager>const RockManager::create()
{
	auto instance = std::make_shared<RockManager>();
	GameObjectTask::getInstnace().add(instance);
	return instance;
}

void RockManager::update()
{

	if(create_count_ % (60 * 5) == 0)
	{
		float area = 60;

		BubbleManager::create(Vec3f(Rand::randFloat(-area, area), 0, Rand::randFloat(-area, area)));
	}

	create_count_++;
}