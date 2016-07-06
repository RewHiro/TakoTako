#include "PlayerJumper.h"

#include "input/UtilityGamePad.h"

#include "gameobject/Player.h"

#include "Transform.h"
#include "BoxCollider.h"

using namespace input;
using namespace ci;

const float PlayerJumper::JUMP_POWER = 1.0f;
const float PlayerJumper::GRAVITY_POWER = 0.04f;
const float PlayerJumper::LIMIT_POS_Y = -0.5f;

PlayerJumper::PlayerJumper(GameObject& gameobject) :
Component(gameobject),
id_(0),
jump_power_(0.0f),
is_jump_(false),
is_rock_hit_(false)
{

}

void PlayerJumper::start()
{
	player_ = gameObject().getObject<Player>();
	id_ = static_cast<int>(player_.lock()->getType()) - 1;
}

void PlayerJumper::update()
{
	auto player_state = gameObject().getObject<Player>()->getState();
	if (!(player_state == Player::State::LIVE ||
		player_state == Player::State::DAMAGE || 
		player_state == Player::State::MELEE_ATTACK))return;
	jump();
	gravity();
}

void PlayerJumper::onCollisionEnter(GameObject& gameobject)
{
	if (gameobject.getHashCode() != std::hash<std::string>()("Rock"))return;
	auto player_state = gameObject().getObject<Player>()->getState();
	if (!(player_state == Player::State::LIVE ||
		player_state == Player::State::DAMAGE ||
		player_state == Player::State::MELEE_ATTACK))return;
	auto collider = gameobject.getComponent<BoxCollider>();
	auto player_collider = gameObject().getComponent<BoxCollider>();

	if (collider->debugPos().y + collider->debugScale().y * 0.5f >= player_collider->debugPos().y - player_collider->debugScale().y * 0.2f)return;
	is_jump_ = false;
	jump_power_ = 0.0f;

}

void PlayerJumper::onCollisionExit(GameObject& gameobject)
{

}

void PlayerJumper::momentCollisionExit(GameObject& gameobject)
{
	if (gameobject.getHashCode() != std::hash<std::string>()("Rock"))return;

	if (!(gameObject().getObject<Player>()->getState() == Player::State::LIVE ||
		gameObject().getObject<Player>()->getState() == Player::State::DAMAGE || 
		gameObject().getObject<Player>()->getState() == Player::State::MELEE_ATTACK))return;

	is_jump_ = true;

}

void PlayerJumper::jump()
{
	if (!(player_.lock()->getState() == Player::State::LIVE || 
		player_.lock()->getState() == Player::State::DAMAGE))return;

	if (is_jump_)return;
	const auto& gamepad = GamePad::getInstance();
	if (gamepad.isPushButton(id_, GamePad::ButtonID::DOWN))
	{
		is_jump_ = !is_jump_;
		jump_power_ = JUMP_POWER;
	}
}

void PlayerJumper::gravity()
{
	if (!is_jump_)return;
	auto transform = gameObject().transform();
	jump_power_ -= GRAVITY_POWER;
	transform->translate(Vec3f(0, jump_power_, 0));

	if (transform->getPos().y <= LIMIT_POS_Y)
	{
		transform->setPos(Vec3f(transform->getPos().x, LIMIT_POS_Y, transform->getPos().z));
		is_jump_ = !is_jump_;
	}
}