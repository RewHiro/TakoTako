#include "PlayerMeleeAttacker.h"

#include "cinder/app/App.h"

#include "input/UtilityGamePad.h"

#include "gameobject/Player.h"

#include "Transform.h"
#include "BoxCollider.h"

using namespace input;
using namespace ci;
using namespace ci::app;

PlayerMeleeAttacker::PlayerMeleeAttacker(GameObject& gameobject) :
Component(gameobject),
id_(0),
speed_(Vec3f::zero())
{

}

void PlayerMeleeAttacker::start()
{
	player_ = gameObject().getObject<Player>();
	id_ = static_cast<int>(player_.lock()->getType()) - 1;
}

void PlayerMeleeAttacker::update()
{
	attackAction();
	inAttack();
}

void PlayerMeleeAttacker::onCollisionEnter(GameObject& gameobject)
{
	if (gameobject.getHashCode() != std::hash<std::string>()("Rock"))return;

	if (player_.lock()->getAnimationState() != Player::AnimationState::MELEE_ATTACK)return;

	auto collider = gameobject.getComponent<BoxCollider>();
	auto player_collider = gameObject().getComponent<BoxCollider>();


	if (collider->debugPos().y + collider->debugScale().y * 0.5f <= player_collider->debugPos().y - player_collider->debugScale().y * 0.2f)return;

	gameObject().transform()->translate(-speed_);
}

void PlayerMeleeAttacker::onCollisionExit(GameObject& gameobject)
{
	if (gameobject.getHashCode() != std::hash<std::string>()("Background"))return;

	if (player_.lock()->getAnimationState() != Player::AnimationState::MELEE_ATTACK)return;

	gameObject().transform()->translate(-speed_);
}

void PlayerMeleeAttacker::attackAction()
{
	if (!(player_.lock()->getState() == Player::State::LIVE ||
		player_.lock()->getState() == Player::State::DAMAGE))return;
	const auto& gamepad = GamePad::getInstance();
	if (gamepad.isPushButton(id_, GamePad::ButtonID::UP))
	{
		auto direction = gameObject().transform()->getRotate() * ci::Vec3f::zAxis();
		auto norm_direction = direction.safeNormalized();

		speed_ = norm_direction *0.8f;

		player_.lock()->changeAnimationState(Player::AnimationState::MELEE_ATTACK);
		//player_.lock()->changeMeleeAttackState();
	}
}

void PlayerMeleeAttacker::inAttack()
{
	if (player_.lock()->getAnimationState() != Player::AnimationState::MELEE_ATTACK)return;
	gameObject().transform()->translate(speed_);
}