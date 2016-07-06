#include "GroundManager.h"

#include "GameObjectTask.h"

#include "Ground.h"

#include "component/Transform.h"

using namespace ci;

const int GroundManager::BLOCK_NUM = 25;


GroundManager::GroundManager() :
GameObject("GroundManager")
{
	//auto scale = 5;
	//auto div_pos = BLOCK_NUM * 0.5f;
	//for (auto i = 0; i < BLOCK_NUM; ++i)
	//{
	//	for (auto j = 0; j < BLOCK_NUM; ++j)
	//	{
	//		auto ground = Ground::create(Vec3f(i + i * scale * 0.5f, 0, j + j *scale * 0.5f), Vec3f(scale, 1, scale));
	//		ground->transform()->setParent(transform_);
	//	}
	//}
	auto scale = 230;
	auto ground = Ground::create(Vec3f::zero(), Vec3f(scale, 50, scale));
	ground->transform()->setParent(transform_);
}

std::shared_ptr<GroundManager>const GroundManager::create()
{
	auto instance = std::make_shared<GroundManager>();
	GameObjectTask::getInstnace().add(instance);
	return instance;
}