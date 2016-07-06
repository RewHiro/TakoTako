#include "PlayerDrainAttack.h"

#include "scene/Scene.h"

#include "GameObjectTask.h"
#include "Player.h"
#include "Item.h"

#include "component/SphereCollider.h"
#include "component/Transform.h"
#include "component/PlayerShot.h"

using namespace ci;

PlayerDrainAttack::PlayerDrainAttack():
GameObject("PlayerDrainAttack",Vec3f(0,2.5f,6.0f),Vec3f::one() *4)
{

	auto sphere_collider = std::make_shared<SphereCollider>(*this);
	addComponent(sphere_collider);
}

std::shared_ptr<PlayerDrainAttack>const PlayerDrainAttack::create()
{
	auto instance = std::make_shared<PlayerDrainAttack>();
	GameObjectTask::getInstnace().add(instance);
	return instance;
}

void PlayerDrainAttack::start()
{
	player_ = transform_->getTopOfLevelParent()->gameObject().getObject<Player>();
	player_type_ = player_.lock()->getType();
}

void PlayerDrainAttack::draw()
{
	//gl::pushModelView();
	//gl::multModelView(getComponent<SphereCollider>()->debugMatrix());

	//gl::drawSphere(Vec3f::zero(), 1.0f);

	//gl::popModelView();
}

void PlayerDrainAttack::onCollisionEnter(GameObject& gameobject)
{
	if (player_.lock()->getAnimationState() != Player::AnimationState::DRAIN)return;
	if (gameobject.getHashCode() != std::hash<std::string>()("Item"))return;
	auto item = gameobject.getObject<Item>();
	if (item->isDrained())return;
	onItemBulletEnter(item);
	onItemInvincibleEnter(item);
}

void PlayerDrainAttack::onCollisionExit(GameObject& gameobject)
{

}

void PlayerDrainAttack::onItemBulletEnter(std::weak_ptr<Item> item)
{
	if (!item.lock()->isBullet())return;
	player_.lock()->getComponent<PlayerShot>()->addBulletCount(10);
	item.lock()->changeDrainedState(player_.lock());
}

void PlayerDrainAttack::onItemInvincibleEnter(std::weak_ptr<Item> item)
{
	if (!item.lock()->isInvincible())return;
	player_.lock()->changeInbincible();
	item.lock()->changeDrainedState(player_.lock());
}