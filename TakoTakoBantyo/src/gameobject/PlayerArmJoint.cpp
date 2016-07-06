#include "PlayerArmJoint.h"

#include "GameObjectTask.h"
#include "Player.h"

#include "component/Transform.h"

using namespace ci;

PlayerArmJoint::PlayerArmJoint(const ci::Vec3f& pos, const Type type) :
GameObject("PlayerArmJoint", pos, Vec3f(0.7f, 1, 1)),
angle_(0.0f),
theta_(0.0f),
type_(type)
{
	if (type == Type::RIGHT)
	{
		transform_->setRotate(Quatf(Vec3f::zAxis(), 1.0f));
	}
}

std::shared_ptr<PlayerArmJoint>const PlayerArmJoint::create(const ci::Vec3f& pos, const Type type)
{
	auto insatnce = std::make_shared<PlayerArmJoint>(pos, type);
	GameObjectTask::getInstnace().add(insatnce);
	return insatnce;
}

void PlayerArmJoint::start()
{
	player_ = transform_->getTopOfLevelParent()->gameObject().getObject<Player>();
}

void PlayerArmJoint::update()
{
	notAttackAnimation();
	meleeAttackAnimation();
}

void PlayerArmJoint::notAttackAnimation()
{
	if (Player::AnimationState::MELEE_ATTACK == player_.lock()->getAnimationState())return;
	waitAnimation();
	moveAnimation();
}

void PlayerArmJoint::waitAnimation()
{
	if (Player::AnimationState::WAIT != player_.lock()->getAnimationState())return;
	theta_ += 0.01f;
	auto max_amplitude = 0.0f;
	max_amplitude = Type::LEFT == type_ ? -0.4f : 0.4f;

	auto diff_amplitude = 0.0f;
	diff_amplitude = Type::LEFT == type_ ? -1.0f : 1.0f;

	angle_ = std::fabs(std::sinf(theta_)) * max_amplitude + diff_amplitude;
	transform_->setRotate(Quatf(Vec3f::zAxis(), angle_));
}

void PlayerArmJoint::moveAnimation()
{
	if (Player::AnimationState::MOVE != player_.lock()->getAnimationState())return;
	theta_ += 0.08f;
	auto angle = std::sinf(theta_) * 0.8f;
	if(Type::LEFT == type_)
	{
		angle *= -1;
	}
	transform_->setRotate(Quatf(Vec3f::zAxis(), angle_) * Quatf(Vec3f::xAxis(), angle));
}

void PlayerArmJoint::meleeAttackAnimation()
{
	if (Player::AnimationState::MELEE_ATTACK != player_.lock()->getAnimationState())return;

	theta_ += 0.2f;
	auto angle = std::fabs(std::sinf(theta_)) * 3.14f * 2;
	if (Type::LEFT == type_)
	{
		angle += 3.14f;
	}
	transform_->setRotate(Quatf(Vec3f::zAxis(), angle_) * Quatf(Vec3f::xAxis(), angle));
}