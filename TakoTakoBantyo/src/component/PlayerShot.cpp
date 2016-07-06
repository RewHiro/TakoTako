#include "PlayerShot.h"

#include "input/UtilityGamePad.h"

#include "gameobject/Player.h"
#include "gameobject/PlayerBullet.h"

#include "Transform.h"
#include "PlayerMover.h"

using namespace ci;
using namespace ci::app;
using namespace input;

const int PlayerShot::MAX_BULLE_NUM = 100;

PlayerShot::PlayerShot(GameObject& gameobject) :
Component(gameobject),
shot_count_(0.0f),
id_(0),
bullet_count_(50)
{

	auto source_file = audio::load(loadAsset("shot.wav"));
	se_ = ci::audio::Voice::create(source_file);
}

void PlayerShot::start()
{
	player_ = gameObject().getObject<Player>();
	id_ = static_cast<int>(player_.lock()->getType()) - 1;
}

void PlayerShot::update()
{
	shot();
	countLimit();
}

void PlayerShot::onCollisionEnter(GameObject& gameobject)
{
	if (gameobject.getHashCode() != std::hash<std::string>()("Item"))return;
}

void PlayerShot::onCollisionExit(GameObject& gameobject)
{

}

void PlayerShot::shot()
{

	if (bullet_count_ <= 0)return;
	if (!(player_.lock()->getState() == Player::State::LIVE ||
		player_.lock()->getState() == Player::State::DAMAGE))return;
	const auto& gamepad = GamePad::getInstance();

	if (gamepad.isPressButton(id_, GamePad::ButtonID::RIGHT))
	{

		if (static_cast<int>(shot_count_) % 10 == 0)
		{
			se_->start();
			--bullet_count_;
			auto direction = gameObject().transform()->getRotate() * ci::Vec3f::zAxis();
			PlayerBullet::create(gameObject().transform()->getPos(), direction, player_.lock()->getType(), gameObject().transform()->getRotate(), static_cast<int>(player_.lock()->getType()));
		}
		shot_count_ += 1.0f;
	}
	else
	{
		shot_count_ = 0.0f;
	}
}

void PlayerShot::countLimit()
{
	if (bullet_count_ < MAX_BULLE_NUM)return;
	bullet_count_ = MAX_BULLE_NUM;
}