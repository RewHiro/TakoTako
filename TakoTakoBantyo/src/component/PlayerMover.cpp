#include  "PlayerMover.h"

#include "input/UtilityGamePad.h"

#include "Transform.h"
#include "BoxCollider.h"
#include "PlayerJumper.h"

#include "gameobject/Player.h"
#include "gameobject/Camera.h"

using namespace ci;
using namespace input;

PlayerMover::PlayerMover(GameObject& gameobject):
Component(gameobject),
direction_(Vec3f::zero()),
moving_distance_(Vec3f::zero()),
id_(0)
{

}


void PlayerMover::start()
{
	std::string camera_manager_str = "CameraManager";
	for (auto& child : gameObject().transform()->getChildren())
	{
		if (child->gameObject().getHashCode() != std::hash<std::string>()(camera_manager_str))continue;
		camera_manager_ = &(child->gameObject());
	}

	for(auto child : camera_manager_->transform()->getChildren())
	{
		camera_ = child->gameObject().getObject<object::Camera>();
	}
	player_ = gameObject().getObject<Player>();
	id_ = static_cast<int>(player_.lock()->getType()) - 1;
}

void PlayerMover::update()
{
	moving_distance_ = Vec3f::zero();
	if (!(player_.lock()->getState() == Player::State::LIVE || 
		player_.lock()->getState() == Player::State::DAMAGE))return;
	if (player_.lock()->getAnimationState() == Player::AnimationState::DRAIN)return;
	if (player_.lock()->getAnimationState() == Player::AnimationState::MELEE_ATTACK)return;

	const auto& gamepad = GamePad::getInstance();

	auto direction = camera_manager_->transform()->getRotate() * Vec3f::zAxis();
	
	direction = Vec3f(camera_.lock()->getCameraPersp().getViewDirection().x, 0, camera_.lock()->getCameraPersp().getViewDirection().z);
	direction.safeNormalize();
	direction_ = direction;

	auto cross = -direction.cross(Vec3f::yAxis());
	cross.safeNormalize();

	cross *= -gamepad.getPressAxis(id_, GamePad::AxisID::LEFT_X);
	direction *= -gamepad.getPressAxis(id_, GamePad::AxisID::LEFT_Y);
	auto speed = (cross + direction).safeNormalized();


	auto player_anim_state = Player::AnimationState::WAIT;
	if (Vec3f::zero() != speed)
	{
		player_anim_state = Player::AnimationState::MOVE;
		gameObject().transform()->setRotate(Quatf(Vec3f::zAxis(), speed));
	}

	player_.lock()->changeAnimationState(player_anim_state);
	moving_distance_ = speed * 0.3f;
	gameObject().transform()->translate(moving_distance_);
}

void PlayerMover::onCollisionEnter(GameObject& gameobject)
{
	if (gameobject.getHashCode() != std::hash<std::string>()("Rock"))return;
	auto player_state = gameObject().getObject<Player>()->getState();
	if (!(player_state == Player::State::LIVE ||
		player_state == Player::State::DAMAGE ||
		player_state == Player::State::MELEE_ATTACK))return;

	if (gameObject().getObject<Player>()->getAnimationState() == Player::AnimationState::DRAIN)return;

	auto collider = gameobject.getComponent<BoxCollider>();
	auto player_collider = gameObject().getComponent<BoxCollider>();

	if (collider->debugPos().y + collider->debugScale().y * 0.5f <= player_collider->debugPos().y - player_collider->debugScale().y * 0.2f)return;
	gameObject().transform()->translate(-moving_distance_);
}

void PlayerMover::onCollisionExit(GameObject& gameobject)
{
	if (gameobject.getHashCode() != std::hash<std::string>()("Background"))return;
	auto player_state = gameObject().getObject<Player>()->getState();

	if (!(player_state == Player::State::LIVE ||
		player_state == Player::State::DAMAGE || 
		player_state == Player::State::MELEE_ATTACK))return;

	if(gameObject().getObject<Player>()->getAnimationState() == Player::AnimationState::DRAIN)return;

	if (gameObject().getComponent<PlayerJumper>()->isJump() &&
		gameObject().getObject<Player>()->getState() == Player::State::MELEE_ATTACK)return;

	gameObject().transform()->translate(-moving_distance_);
}