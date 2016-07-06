#include "PlayerArmManager.h"

#include "ResourceManager.h"

#include "GameObjectTask.h"
#include "PlayerLeftArm.h"
#include "PlayerRightArm.h"
#include "PlayerArmJoint.h"

#include "component/Transform.h"

using namespace ci;

PlayerArmManager::PlayerArmManager() :
GameObject("PlayerArmManager")
{
	auto& loader = ResourceManager::getInstance().find(ModelType::PLAYER_FOOT);

	TriMesh player_right_arm_mesh;
	loader.load(
		6,
		&player_right_arm_mesh,
		boost::logic::indeterminate,
		boost::logic::indeterminate,
		false
		);

	auto player_right_arm_joint = PlayerArmJoint::create(Vec3f(-1, 3, 0), PlayerArmJoint::Type::RIGHT);
	player_right_arm_joint->transform()->setParent(transform_);

	auto player_right_arm = PlayerRightArm::create(player_right_arm_mesh);
	player_right_arm->transform()->setParent(player_right_arm_joint->transform());

	TriMesh player_left_arm_mesh;
	loader.load(
		7,
		&player_left_arm_mesh,
		boost::logic::indeterminate,
		boost::logic::indeterminate,
		false
		);

	auto player_left_arm_joint = PlayerArmJoint::create(Vec3f(1, 3, 0),PlayerArmJoint::Type::LEFT);
	player_left_arm_joint->transform()->setParent(transform_);

	auto player_left_arm = PlayerLeftArm::create(player_left_arm_mesh);
	player_left_arm->transform()->setParent(player_left_arm_joint->transform());

}

std::shared_ptr<PlayerArmManager>const PlayerArmManager::create()
{
	auto instance = std::make_shared<PlayerArmManager>();
	GameObjectTask::getInstnace().add(instance);
	return instance;
}