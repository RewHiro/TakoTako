#include "PlayerFootManager.h"

#include "ResourceManager.h"

#include "PlayerFoot.h"
#include "PlayerFootJoint.h"
#include "GameObjectTask.h"

#include "component/Transform.h"

using namespace ci;

PlayerFootManager::PlayerFootManager() :
GameObject("PlayerFootManager")
{

	auto& loader = ResourceManager::getInstance().find(ModelType::PLAYER_FOOT);

	for (int i = 0; i < loader.getNumGroups() - 2; ++i)
	{

		TriMesh mesh;
		loader.load(
			i,
			&mesh,
			boost::logic::indeterminate,
			boost::logic::indeterminate,
			false
			);

		std::shared_ptr<PlayerFootJoint>player_foot_joint;
		std::shared_ptr<PlayerFoot>foot;
		switch (i)
		{
			case 0:
				player_foot_joint = PlayerFootJoint::create(Vec3f(0.5f, 3.5f, 0), 0.1f);
				player_foot_joint->transform()->setParent(transform_);

				foot = PlayerFoot::create(i + 1, mesh, Vec3f(-0.5f, -3.5f, 0));
				foot->transform()->setParent(player_foot_joint->transform());
				break;
			case 1:
				player_foot_joint = PlayerFootJoint::create(Vec3f(0.5f, 3.5f, 1),0.15f);
				player_foot_joint->transform()->setParent(transform_);

				foot = PlayerFoot::create(i + 1, mesh, Vec3f(-0.5f, -3.5f, -1));
				foot->transform()->setParent(player_foot_joint->transform());
				break;
			case 2:
				player_foot_joint = PlayerFootJoint::create(Vec3f(-0.5f, 3.5f, 1), 0.15f);
				player_foot_joint->transform()->setParent(transform_);

				foot = PlayerFoot::create(i + 1, mesh, Vec3f(0.5f, -3.5f, -1));
				foot->transform()->setParent(player_foot_joint->transform());
				break;
			case 3:
				player_foot_joint = PlayerFootJoint::create(Vec3f(-0.5f, 3.5f, 0), 0.1f);
				player_foot_joint->transform()->setParent(transform_);

				foot = PlayerFoot::create(i + 1, mesh, Vec3f(0.5f, -3.5f, 0));
				foot->transform()->setParent(player_foot_joint->transform());
				break;
			case 4:
				player_foot_joint = PlayerFootJoint::create(Vec3f(-0.5f, 3.5f, -1), 0.5f);
				player_foot_joint->transform()->setParent(transform_);

				foot = PlayerFoot::create(i + 1, mesh, Vec3f(0.5f, -3.5f, 1));
				foot->transform()->setParent(player_foot_joint->transform());
				break;

			case 5:
				player_foot_joint = PlayerFootJoint::create(Vec3f(0.5f, 3.5f, -1), 0.5f);
				player_foot_joint->transform()->setParent(transform_);

				foot = PlayerFoot::create(i + 1, mesh, Vec3f(-0.5f, -3.5f, 1));
				foot->transform()->setParent(player_foot_joint->transform());
				break;
		}




	}
}

std::shared_ptr<PlayerFootManager>const PlayerFootManager::create()
{
	auto instance = std::make_shared<PlayerFootManager>();
	GameObjectTask::getInstnace().add(instance);
	return instance;
}