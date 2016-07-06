#include "CrabFootManager.h"

#include "ResourceManager.h"

#include "CrabFootJoint.h"
#include "CrabFoot.h"
#include "GameObjectTask.h"

#include "component/Transform.h"

using namespace ci;

CrabFootManager::CrabFootManager() :
GameObject("CrabFootManager")
{

	auto& loader = ResourceManager::getInstance().find(ModelType::ITEM01_ARM);

	for (auto i = 0; i < loader.getNumGroups() - 2; ++i)
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

			case 0:
				pos = Vec3f(0.5f, 0.8f, 0);
				break;

			case 1:
				pos = Vec3f(0.5f, 0.5f, 0.2f);
				break;

			case 2:
				pos = Vec3f(0.5f, 0.4f, 0.3f);
				break;

			case 3:
				pos = Vec3f(-0.5f, 0.8f, 0);
				break;

			case 4:
				pos = Vec3f(-0.5f, 0.5f, 0.2f);
				break;

			case 5:
				pos = Vec3f(-0.5f, 0.4f, 0.3f);
				break;
		}
		auto crab_foot_joint = CrabFootJoint::create(pos, i);
		crab_foot_joint->transform()->setParent(transform_);

		auto crab_foot = CrabFoot::create(-pos, mesh);
		crab_foot->transform()->setParent(crab_foot_joint->transform());
	}
}

std::shared_ptr<CrabFootManager>const CrabFootManager::create()
{
	auto insatance = std::make_shared<CrabFootManager>();
	GameObjectTask::getInstnace().add(insatance);
	return insatance;
}