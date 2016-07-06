#include "CrabArmJoint.h"

#include "GameObjectTask.h"
#include "Item.h"

#include "component/Transform.h"

using namespace ci;

CrabArmJoint::CrabArmJoint(const ci::Vec3f& pos, int num) :
GameObject("CrabArmJoint", pos),
count_(0.0f),
num_(num)
{

}

std::shared_ptr<CrabArmJoint>const CrabArmJoint::create(const ci::Vec3f& pos, int num)
{
	auto insatnce = std::make_shared<CrabArmJoint>(pos, num);
	GameObjectTask::getInstnace().add(insatnce);
	return insatnce;
}

void CrabArmJoint::start()
{
	item_ = transform_->getTopOfLevelParent()->gameObject().getObject<Item>();
}

void CrabArmJoint::update()
{
	inMove();
}

void CrabArmJoint::inMove()
{
	if (!item_.lock()->isMove())return;

	float radian = 0.0f;
	switch (num_)
	{
		case 6:
			radian = std::sinf(count_) * M_PI * 0.05f;
			transform_->setRotate(Quatf(Vec3f::zAxis(), radian));
			break;
		case 7:
			radian = std::sinf(count_ + 3.14f) * M_PI * 0.05f;
			transform_->setRotate(Quatf(Vec3f::zAxis(), radian));
			break;
	}
	count_ += 0.09f;
}