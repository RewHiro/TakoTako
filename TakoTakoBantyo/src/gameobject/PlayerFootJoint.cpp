#include "PlayerFootJoint.h"

#include "cinder/Rand.h"

#include "GameObjectTask.h"
#include "Player.h"

#include "component/Transform.h"

using namespace ci;
using namespace ci::app;

PlayerFootJoint::PlayerFootJoint(const ci::Vec3f& pos, float theta) :
GameObject("PlayerFootJoint", pos, Vec3f::one()),
DIFF_THETA(theta),
theta_(0.0f),
angle_(0.0f),
drained_count_(0.0f),
is_wait_(true),
is_drained_(false)
{

}

std::shared_ptr<PlayerFootJoint>const PlayerFootJoint::create(const ci::Vec3f& pos, float theta)
{
	auto insatnce = std::make_shared<PlayerFootJoint>(pos, theta);
	GameObjectTask::getInstnace().add(insatnce);
	return insatnce;
}

void PlayerFootJoint::startDrained(const ci::Vec3f& drain_pos)
{
	auto pos = transform_->worldPos();
	transform_->resetParent();
	transform_->setPos(pos);
	timeline().apply(&transform()->getAnimPos(), drain_pos + Vec3f::yAxis() * 5, 2.5f, EaseInCirc());
	timeline().apply(&transform()->getAnimScale(), Vec3f::zero(), 2.5f, EaseInBack(8));

	is_drained_ = true;
}

void PlayerFootJoint::start()
{
	player_ = transform_->getTopOfLevelParent()->gameObject().getObject<Player>();
}

void PlayerFootJoint::update()
{
	waitAnimation();
	moveAnimation();
	meleeAttackAnimation();
	drawAnimmation();
}

void PlayerFootJoint::waitAnimation()
{
	if (is_wait_)return;
	if (Player::AnimationState::WAIT != player_.lock()->getAnimationState())return;
	theta_ = 0.0f;
	angle_ = 0.0f;
	transform_->setRotate(Quatf(Vec3f::xAxis(), angle_));
	is_wait_ = true;
}

void PlayerFootJoint::moveAnimation()
{
	if (Player::AnimationState::MOVE != player_.lock()->getAnimationState())return;

	is_wait_ = false;

	theta_ += 0.08f;
	angle_ = std::sinf(theta_ + DIFF_THETA) * 0.15f;

	transform_->setRotate(Quatf(Vec3f::xAxis(), angle_));
}

void PlayerFootJoint::meleeAttackAnimation()
{
	if (is_wait_)return;
	if (Player::AnimationState::MELEE_ATTACK != player_.lock()->getAnimationState())return;
	theta_ = 0.0f;
	angle_ = 0.0f;
	transform_->setRotate(Quatf(Vec3f::xAxis(), angle_));
	is_wait_ = true;
}

void PlayerFootJoint::drawAnimmation()
{
	if (!is_drained_)return;
	drained_count_ += 1.0f;
	transform_->rotate(Quatf(Rand::randVec3f(), Rand::randFloat(0.0f, M_PI)));

	if (drained_count_ < 60 * 2.5f)return;
	destroy();
	for(auto &child : transform_->getChildren())
	{
		child->gameObject().destroy();
	}
}