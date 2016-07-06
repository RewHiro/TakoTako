#include "CrabFootJoint.h"

#include "GameObjectTask.h"
#include "Item.h"

#include "component/Transform.h"

using namespace ci;

CrabFootJoint::CrabFootJoint(const ci::Vec3f& pos, int num) :
GameObject("CrabFootJoint", pos),
move_count_(0.0f),
num_(num)
{
	switch(num_ % 3)
	{
		case 0:
			DIFF_THETA = 0.9f;
			break;
		case 1:
			DIFF_THETA = 0.6f;
			break;
		case 2:
			DIFF_THETA = 0.3f;
			break;
	}

}

std::shared_ptr<CrabFootJoint>const CrabFootJoint::create(const ci::Vec3f& pos, int num)
{
	auto instance = std::make_shared<CrabFootJoint>(pos, num);
	GameObjectTask::getInstnace().add(instance);
	return instance;
}

void CrabFootJoint::start()
{
	item_ = transform_->getTopOfLevelParent()->gameObject().getObject<Item>();
}

void CrabFootJoint::update()
{
	inMove();
}

void CrabFootJoint::inMove()
{
	if (!item_.lock()->isMove())return;
	move_count_ += 0.01;
	auto radian = std::sinf(move_count_ + DIFF_THETA) * M_PI * item_.lock()->getSpeed().length();
	transform_->setRotate(Quatf(Vec3f::zAxis(), radian));
}
