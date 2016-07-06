#include "BubbleManager.h"

#include "Bubble.h"
#include "GameObjectTask.h"

#include "component/Transform.h"

using namespace ci;

BubbleManager::BubbleManager(const ci::Vec3f& pos) :
GameObject("BubbleManager"),
SPEED(0.08f),
SWING(5.0f),
center_point_(pos),
create_count_(0)
{
	auto bubble = Bubble::create(pos, Vec3f::one(), SWING, SPEED);
	bubble->transform()->setParent(transform_);
}

std::shared_ptr<BubbleManager>const BubbleManager::create(const ci::Vec3f& pos)
{
	auto instance = std::make_shared<BubbleManager>(pos);
	GameObjectTask::getInstnace().add(instance);
	return instance;
}

void BubbleManager::update()
{
	create_count_++;

	if(create_count_ == 60)
	{
		auto bubble = Bubble::create(center_point_, Vec3f::one(), SWING, SPEED);
		bubble->transform()->setParent(transform_);
	}

	if (create_count_ == 120)
	{
		auto bubble = Bubble::create(center_point_, Vec3f::one(), SWING, SPEED);
		bubble->transform()->setParent(transform_);
	}

	if(transform_->getChildren().size() == 0)
	{
		destroy();
	}
}