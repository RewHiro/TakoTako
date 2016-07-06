#include "CrabArmManager.h"

#include "ResourceManager.h"

#include "GameObjectTask.h"
#include "CrabArmJoint.h"
#include "CrabArm.h"

#include "component/Transform.h"

using namespace ci;

CrabArmManager::CrabArmManager():
GameObject("CrabArmManager")
{
	auto& loader = ResourceManager::getInstance().find(ModelType::ITEM01_ARM);

	for (auto i = 6; i < loader.getNumGroups(); ++i)
	{
		TriMesh mesh;

		loader.load(
			i,
			&mesh,
			boost::logic::indeterminate,
			boost::logic::indeterminate,
			false
			);

		Vec3f pos = Vec3f::zero();

		switch (i)
		{

			case 6:
				pos = Vec3f(0.6f, 0.8f, 0);
				break;

			case 7:
				pos = Vec3f(-0.6f, 0.8f, 0);
				break;

		}
		auto crab_arm_joint = CrabArmJoint::create(pos, i);
		crab_arm_joint->transform()->setParent(transform_);

		auto crab_arm = CrabArm::create(-pos, mesh);
		crab_arm->transform()->setParent(crab_arm_joint->transform());
	}
}

std::shared_ptr<CrabArmManager>const CrabArmManager::create()
{
	auto instance = std::make_shared<CrabArmManager>();
	GameObjectTask::getInstnace().add(instance);
	return instance;
}