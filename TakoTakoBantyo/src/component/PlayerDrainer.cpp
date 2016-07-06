#include "PlayerDrainer.h"

#include "input/UtilityGamePad.h"

#include "gameobject/Player.h"

#include "Transform.h"

using namespace input;
using namespace ci;
using namespace ci::app;

PlayerDrainer::PlayerDrainer(GameObject& gameobject):
Component(gameobject),
id_(0),
count_(0.0f)
{
	auto source_file = audio::load(loadAsset("drain.wav"));
	se_ = ci::audio::Voice::create(source_file);
}


void PlayerDrainer::start()
{
	player_ = gameObject().getObject<Player>();
	id_ = static_cast<int>(player_.lock()->getType()) - 1;
}

void PlayerDrainer::update()
{
	drainAction();
}

void PlayerDrainer::drainAction()
{
	if (!(player_.lock()->getState() == Player::State::LIVE ||
		player_.lock()->getState() == Player::State::DAMAGE))return;
	if (player_.lock()->getAnimationState() == Player::AnimationState::MELEE_ATTACK)return;

	const auto& gamepad = GamePad::getInstance();
	if (gamepad.isPressButton(id_, GamePad::ButtonID::LEFT))
	{
		if(!se_->isPlaying())
		{
			se_->start();
		}
		count_ += 0.5f;
		gameObject().transform()->setScale(Vec3f(1, std::sinf(count_)*0.2f + 1.0f, 1));
		player_.lock()->changeAnimationState(Player::AnimationState::DRAIN);
	}
	else
	{
		count_ = 0.0f;
		gameObject().transform()->setScale(Vec3f::one());
		player_.lock()->changeAnimationState(Player::AnimationState::WAIT);
	}
}