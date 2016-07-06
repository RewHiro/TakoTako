#include "Player.h"

#include "cinder/Rand.h"

#include "ResourceManager.h"

#include "scene/Scene.h"

#include "input/UtilityGamePad.h"

#include "CameraManager.h"
#include "GameObjectTask.h"
#include "PlayerFootManager.h"
#include "PlayerArmManager.h"
#include "PlayerDrainAttack.h"
#include "PlayerFootJoint.h"
#include "DamageEffectManager.h"
#include "PlayerBullet.h"
#include "Camera.h"

#include "component/Transform.h"
#include "component/PlayerMover.h"
#include "component/PlayerShot.h"
#include "component/PlayerMeleeAttacker.h"
#include "component/PlayerJumper.h"
#include "component/PlayerDrainer.h"
#include "component/BoxCollider.h"
#include "component/SphereCollider.h"

using namespace ci;
using namespace ci::app;
using namespace input;

Player::Player(int player_num, const ci::Vec3f& pos, const ci::Quatf& rotate, const PlayerType type) :
GameObject("Player" + std::to_string(player_num), pos, Vec3f::one(), rotate),
fall_direction_(Vec3f::zero()),
struggle_(Vec3f::zero()),
melee_attack_count_(0.0f),
damage_count_(0.0f),
revive_count_(0.0f),
drain_count_(0.0f),
invincible_count_(0.0f),
fall_count_(0.0f),
drained_count_(0.0f),
axis_value_(Vec2f::zero()),
type_(type),
//texture_(ResourceManager::getInstance().find(TextureType::PLAYER)),
state_(State::LIVE),
animation_state_(AnimationState::WAIT),
hp_(3),
down_count_(0),
id_(player_num - 1),
is_result_(false),
is_invincible_(false)
{
	{
		auto source_file = audio::load(loadAsset("invincible.wav"));
		invincible_se_ = ci::audio::Voice::create(source_file);
	}

	{
		auto source_file = audio::load(loadAsset("hit.wav"));
		hit_se_ = ci::audio::Voice::create(source_file);
	}


	switch (type)
	{
		case PlayerType::ONE:
			texture_ = ResourceManager::getInstance().find(TextureType::PLAYER01);
			break;
		case PlayerType::TWO:
			texture_ = ResourceManager::getInstance().find(TextureType::PLAYER02);
			break;
		case PlayerType::THREE:
			texture_ = ResourceManager::getInstance().find(TextureType::PLAYER03);
			break;
		case PlayerType::FOUR:
			texture_ = ResourceManager::getInstance().find(TextureType::PLAYER04);
			break;
	}

	material_.setDiffuse(ColorA(0.8f, 0.8f, 0.8f));
	material_.setAmbient(material_.getDiffuse()*0.5f);
	material_.setSpecular(ColorA(0.4f, 0.4f, 0.4f));

	ResourceManager::getInstance().find(ModelType::PLAYER_BODY).load(
		&mesh_,
		boost::logic::indeterminate,
		boost::logic::indeterminate,
		false
		);

	auto player_foot_manager = PlayerFootManager::create();
	player_foot_manager->transform()->setParent(transform_);
	player_foot_manager_ = player_foot_manager;

	auto player_arm_manager = PlayerArmManager::create();
	player_arm_manager->transform()->setParent(transform_);

	auto player_drain_attack = PlayerDrainAttack::create();
	player_drain_attack->transform()->setParent(transform_);


	addComponent(std::make_shared<PlayerMover>(*this));
	addComponent(std::make_shared<PlayerShot>(*this));
	addComponent(std::make_shared<PlayerMeleeAttacker>(*this));
	addComponent(std::make_shared<PlayerJumper>(*this));
	addComponent(std::make_shared<PlayerDrainer>(*this));

	auto box_collider = std::make_shared<BoxCollider>(*this);
	box_collider->transform()->setPos(Vec3f(0, 3, 0));
	box_collider->transform()->setScale(Vec3f(3, 3, 3));
	addComponent(box_collider);
}

std::shared_ptr<Player>const Player::create(int player_num, const ci::Vec3f& pos, const ci::Quatf& rotate, const PlayerType type)
{
	auto instance = std::make_shared<Player>(player_num, pos, rotate, type);
	GameObjectTask::getInstnace().add(instance);
	return instance;
}

void Player::start()
{
	auto box_collider = getComponent<BoxCollider>();
	auto rocks_itr = findObjects("Rock");
	for (auto itr = rocks_itr.first; itr != rocks_itr.second; ++itr)
	{
		box_collider->regist(itr->second->getComponent<BoxCollider>());
	}

	auto back_ground = findObject("Background");
	box_collider->regist(back_ground->getComponent<SphereCollider>());

	auto player_num = static_cast<int>(Scene::playerNum());
	for (auto i = 0; i < player_num; ++i)
	{
		if (static_cast<int>(type_) == i + 1)continue;
		box_collider->regist(findObject("Player" + std::to_string(i + 1))->getComponent<BoxCollider>());
	}

	auto plater_drain_attack = findObjects("PlayerDrainAttack");
	for (auto itr = plater_drain_attack.first; itr != plater_drain_attack.second; ++itr)
	{
		box_collider->regist(itr->second->getComponent<SphereCollider>());
	}
}

void Player::update()
{
	meleeAttack();
	inLive();
	inDamage();
	inDown();
	inRevive();
	inDrained();
	inDraining();
	inInvincible();
	inFall();
}

void Player::draw()
{
	gl::enable(GL_TEXTURE_2D);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	gl::pushModelView();

	gl::multModelView(transform_->worldMatrix());

	material_.apply();
	texture_.bind();
	gl::draw(mesh_);
	texture_.unbind();

	gl::popModelView();

	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	gl::disable(GL_TEXTURE_2D);

	gl::pushModelView();

	ci::gl::Material material;
	material.setDiffuse(ColorA(0.4f, 0.4f, 0.4f));
	material.setAmbient(ColorA(0, 0, 0));

	material.apply();

	auto scale = 1.8f * (0.5f / transform_->getPos().y);
	scale = std::min(scale, 1.8f);
	gl::translate(Vec3f(transform_->worldPos().x, 0.5f, transform_->worldPos().z));
	gl::scale(Vec3f(scale, 0.01f, scale));
	gl::drawSphere(Vec3f::zero(), 1.0f);

	gl::popModelView();

#ifdef DEBUG

	gl::pushModelView();
	gl::multModelView(getComponent<BoxCollider>()->debugMatrix());

	gl::drawStrokedCube(Vec3f::zero(), Vec3f::one());

	gl::popModelView();
#endif
}

// “–‚½‚Á‚½ˆ—
void Player::onCollisionEnter(GameObject& gameobject)
{
	GameObject::onCollisionEnter(gameobject);

	onDamage(gameobject);
	onDrained(gameobject);
	onCollisionEnterRockInFall(gameobject);
}

void Player::onCollisionExit(GameObject& gameobject)
{
	GameObject::onCollisionExit(gameobject);
	onCollisionExitBackGroundInDown(gameobject);
}

void Player::meleeAttack()
{
	if (AnimationState::MELEE_ATTACK != animation_state_)return;
	melee_attack_count_ += 1.0f;
	if (melee_attack_count_ >= 30.0f)
	{
		melee_attack_count_ = 0.0f;
		animation_state_ = AnimationState::WAIT;
	}
}

void Player::onDamage(GameObject& gameobject)
{
	if (gameobject.getHashCode() == std::hash<std::string>()("Background"))return;
	if (gameobject.getHashCode() == std::hash<std::string>()("Rock"))return;
	if (is_invincible_)return;
	onCollisionEnterBullet(gameobject);
	onCollisionEnterPlayerAttack(gameobject);
}

void Player::onDrained(GameObject& gameobject)
{
	if (State::DOWN != state_)return;
	if (State::DRAINED == state_)return;
	if (State::DRAINING == state_)return;
	if (gameobject.getHashCode() != std::hash<std::string>()("PlayerDrainAttack"))return;
	auto player = gameobject.transform()->getTopOfLevelParent()->gameObject().getObject<Player>();
	if (player->getAnimationState() != AnimationState::DRAIN)return;
	drained_count_ += 0.5f;
	transform()->setScale(Vec3f(1, std::sinf(drained_count_)*0.2f + 1.0f, 1));
	if (drained_count_ <= 60 * 2 * 0.5f)return;
	state_ = State::DRAINED;
	player->state_ = State::DRAINING;
	drained_count_ = 0.0f;
	for (auto& child : player_foot_manager_.lock()->transform()->getChildren())
	{
		child->gameObject().getObject<PlayerFootJoint>()->startDrained(player->transform()->worldPos());
		break;
	}
}

void Player::onCollisionEnterBullet(GameObject& gameobject)
{
	if (!(state_ == State::LIVE ||
		state_ == State::MELEE_ATTACK))return;

	auto player_num = static_cast<int>(Scene::playerNum());
	for (int i = 0; i < player_num; ++i)
	{
		if (i + 1 == static_cast<int>(type_))continue;
		if (gameobject.getHashCode() == std::hash<std::string>()("PlayerBullet" + std::to_string(i + 1)))
		{
			hit_se_->start();
			hp_ -= 1;
			state_ = State::DAMAGE;
			DamageEffectManager::create(transform_->worldPos(), gameobject.getObject<PlayerBullet>()->getMaterial());

			if (hp_ <= 0)
			{
				state_ = State::FALL;
			}
			break;
		}
	}
}

void Player::onCollisionEnterPlayerAttack(GameObject& gameobject)
{
	if (!(state_ == State::LIVE ||
		state_ == State::MELEE_ATTACK))return;

	auto player_num = static_cast<int>(Scene::playerNum());
	for (auto i = 0; i < player_num; ++i)
	{
		if (gameobject.getHashCode() == std::hash<std::string>()("Player" + std::to_string(i + 1)))
		{
			if (gameobject.getObject<Player>()->getAnimationState() != AnimationState::MELEE_ATTACK)continue;
			hit_se_->start();
			hp_ -= 1;
			state_ = State::DAMAGE;

			DamageEffectManager::create(transform_->worldPos(), gameobject.getObject<Player>()->getMaterial());

			if (hp_ <= 0)
			{
				state_ = State::FALL;
			}

			break;
		}
	}
}

void Player::onCollisionEnterRockInFall(GameObject& gameobject)
{
	if (State::FALL != state_)return;
	if (fall_count_ <= 0.05f * 60 * 1)return;
	if (gameobject.getHashCode() != std::hash<std::string>()("Rock"))return;
	if (!transform_->getAnimPos().isComplete())return;
	transform_->getAnimPos().stop();
	transform_->setPos(transform_->getPos() + Vec3f::yAxis() * 2.5f);
}

void Player::onCollisionExitBackGroundInDown(GameObject& gameobject)
{
	if (!(State::DOWN == state_ || 
		State::REVIVE == state_))return;
	if (gameobject.getHashCode() != std::hash<std::string>()("Background"))return;
	auto direction = Vec3f::zero() - transform_->worldPos();
	direction.y = 0;
	direction.safeNormalize();
	transform_->translate(direction * 0.5f);
	//transform_->translate(struggle_);
}

void Player::inDamage()
{
	if (State::DAMAGE != state_)return;

	damage_count_ += 0.05f;

	float rgb = std::fabs(std::sinf(damage_count_));
	auto color = ColorA(rgb, rgb, rgb);
	material_.setDiffuse(color);
	material_.setAmbient(material_.getDiffuse()*0.5f);

	if (damage_count_ >= 0.05f * 60 * 2)
	{
		material_.setDiffuse(ColorA(0.8f, 0.8f, 0.8f));
		material_.setAmbient(material_.getDiffuse()*0.5f);
		damage_count_ = 0.0f;
		state_ = State::LIVE;
	}

}

void Player::inDown()
{
	if (State::DOWN != state_)return;
	struggle();
}

void Player::inRevive()
{
	if (State::REVIVE != state_)return;

	if (revive_count_ == 0.0f)
	{
		timeline().apply(
			&transform_->getAnimPos(),
			transform_->worldPos() + Vec3f::yAxis() * 5,
			0.5f,
			EaseOutCubic()).finishFn(std::bind(&Player::easingLanding, this));
	}
	revive_count_ += 1.0f;
	if (revive_count_ >= 1.0f * 60 * 1)
	{
		getComponent<BoxCollider>()->transform()->setPos(Vec3f(0, 3, 0));
		state_ = State::LIVE;
		auto direction = transform_->getRotate() * Vec3f::xAxis();
		switch (type_)
		{
			case PlayerType::ONE:
				transform_->setRotate(Quatf(direction.safeNormalized(), 0)*Quatf(Vec3f::yAxis(), 0));
				break;
			case PlayerType::TWO:
				transform_->setRotate(Quatf(direction.safeNormalized(), 0)*Quatf(Vec3f::yAxis(), M_PI));
				break;
			case PlayerType::THREE:
				transform_->setRotate(Quatf(direction.safeNormalized(), 0)*Quatf(Vec3f::yAxis(), M_PI*0.5f));
				break;
			case PlayerType::FOUR:
				transform_->setRotate(Quatf(direction.safeNormalized(), 0)*Quatf(Vec3f::yAxis(), -M_PI*0.5f));
				break;
		}


		material_.setDiffuse(ColorA(0.8f, 0.8f, 0.8f));
		material_.setAmbient(material_.getDiffuse()*0.5f);
		revive_count_ = 0.0f;
	}
	else
	{
		auto direction = transform_->getRotate() * Vec3f::xAxis();
		transform_->rotate(Quatf(direction.safeNormalized(), 3.14f * 0.5f / 60));
	}
}

void Player::inDrained()
{
	if (State::DRAINED != state_)return;

	drain_count_ += 0.5f;
	transform_->setScale(Vec3f(1, std::sinf(drain_count_)*0.2f + 1.0f, 1));

	if (drain_count_ <= 0.5f * 60 * 3)return;

	if (player_foot_manager_.lock()->transform()->getChildren().size() == 0)
	{
		state_ = State::DEATH;
		is_result_ = true;
	}
	else
	{
		hp_ = 3;
		state_ = State::REVIVE;
		drain_count_ = 0.0f;
		down_count_ = 0;
	}

}

void Player::inDraining()
{
	if (State::DRAINING != state_)return;
	drain_count_ += 0.5f;
	transform_->setScale(Vec3f(1, std::sinf(drain_count_)*0.2f + 1.0f, 1));

	if (drain_count_ <= 0.5f * 60 * 3)return;

	state_ = State::LIVE;
	drain_count_ = 0.0f;
}

void Player::inInvincible()
{
	if (!is_invincible_)return;

	if (!invincible_se_->isPlaying())
	{
		invincible_se_->start();
	}

	if (static_cast<int>(invincible_count_) % 5 == 0)
	{
		material_.setDiffuse(ColorA(Rand::randFloat(0.0f, 1.0f), Rand::randFloat(0.0f, 1.0f), Rand::randFloat(0.0f, 1.0f)));
		material_.setAmbient(material_.getDiffuse() * 0.5f);
	}

	invincible_count_ += 1.0f;

	if (invincible_count_ <= 60 * 10)return;
	invincible_count_ = 0.0f;
	is_invincible_ = false;
	material_.setDiffuse(ColorA(0.8f, 0.8f, 0.8f));
	material_.setAmbient(material_.getDiffuse()*0.5f);
}

void Player::inFall()
{
	if (State::FALL != state_)return;
	fall_count_ += 0.05f;

	if (fall_count_ == 0.05f)
	{
		fall_direction_ = transform_->getRotate() * Vec3f::zAxis();
		timeline().apply(
			&transform_->getAnimPos(),
			transform_->worldPos() + Vec3f::yAxis() * 20,
			1.0f,
			EaseOutCubic()).finishFn(std::bind(&Player::easingBounce, this));
	}

	if (fall_count_ >= 0.05f * 60 * 3)
	{
		state_ = State::DOWN;
		fall_count_ = 0.0f;

		getComponent<BoxCollider>()->transform()->setPos(Vec3f(-fall_direction_.x * 3, 0, -fall_direction_.z * 3));
	}
	else
	{
		auto direction_test = getComponent<PlayerMover>()->getDirection();
		auto direction = transform_->getRotate() * Vec3f::xAxis();
		transform_->rotate(Quatf(direction.safeNormalized(), -3.14f * 0.5f / 180));
	}
}

void Player::inLive()
{
	if (State::LIVE != state_)return;
	material_.setDiffuse(ColorA(0.8f, 0.8f, 0.8f));
	material_.setAmbient(material_.getDiffuse()*0.5f);
}

void Player::struggle()
{
	const auto& gamepad = GamePad::getInstance();

	auto down_count = down_count_;

	if (gamepad.getPressAxis(id_, GamePad::AxisID::LEFT_X) != axis_value_.x)
	{
		axis_value_.x = gamepad.getPressAxis(id_, GamePad::AxisID::LEFT_X);
		++down_count_;
	}

	if (gamepad.getPressAxis(id_, GamePad::AxisID::LEFT_Y) != axis_value_.y)
	{
		axis_value_.y = gamepad.getPressAxis(id_, GamePad::AxisID::LEFT_Y);
		++down_count_;
	}

	if (gamepad.isPushButton(id_, GamePad::ButtonID::DOWN))
	{
		++down_count_;
	}

	if (gamepad.isPushButton(id_, GamePad::ButtonID::LEFT))
	{
		++down_count_;
	}

	if (gamepad.isPushButton(id_, GamePad::ButtonID::RIGHT))
	{
		++down_count_;
	}

	if (gamepad.isPushButton(id_, GamePad::ButtonID::UP))
	{
		++down_count_;
	}

	if (down_count_ != down_count)
	{
		struggle_ = Vec3f(Rand::randFloat(-0.1f, 0.1f), 0, Rand::randFloat(-0.1f, 0.1f));
		transform_->translate(struggle_);
	}

	if (down_count_ >= 280)
	{
		state_ = State::REVIVE;
		hp_ = 2;
		down_count_ = 0;
	}
}

void Player::easingBounce()
{
	timeline().apply(
		&transform_->getAnimPos(),
		Vec3f(transform_->worldPos().x, 2.5f, transform_->worldPos().z),
		1.8f,
		EaseOutBounce());
}

void Player::easingLanding()
{
	timeline().apply(
		&transform_->getAnimPos(),
		transform_->worldPos() - Vec3f::yAxis() * 5 - Vec3f::yAxis() * 2.5f,
		0.5f,
		EaseInCubic());
}