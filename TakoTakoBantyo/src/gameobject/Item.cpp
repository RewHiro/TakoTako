#include "Item.h"

#include "cinder/Rand.h"

#include "scene/Scene.h"

#include "ResourceManager.h"

#include "GameObjectTask.h"
#include "CrabFootManager.h"
#include "CrabArmManager.h"
#include "Player.h"

#include "component/Transform.h"
#include "component/SphereCollider.h"
#include "component/BoxCollider.h"

using namespace ci;
using namespace ci::app;

const float Item::SPEED_RAND = 0.2f;

Item::Item(const ci::Vec3f& pos, Type type) :
GameObject("Item", Vec3f(pos.x, -1, pos.z)),
speed_(Vec3f(Rand::randFloat(-SPEED_RAND, SPEED_RAND), 0, Rand::randFloat(-SPEED_RAND, SPEED_RAND))),
appearance_count_(0.0f),
death_count_(0.0f),
invincible_count_(0),
state_(State::APPEARANCE),
type_(type)
{
	if (type_ == Type::INVINCIBLE)
	{
		speed_ = Vec3f(Rand::randFloat(0.3, 0.4f), 0, Rand::randFloat(0.3f, 0.4f));
	}

	ResourceManager::getInstance().find(ModelType::ITEM01_BODY).load(
		&mesh_,
		boost::logic::indeterminate,
		boost::logic::indeterminate,
		false
		);

	texture_ = ResourceManager::getInstance().find(TextureType::ITEM01_BODY);

	material_.setDiffuse(ColorA(0.8f, 0.8f, 0.8f));
	material_.setAmbient(material_.getDiffuse()*0.5f);
	material_.setSpecular(ColorA(0.4f, 0.4f, 0.4f));

	auto crab_foot_manager = CrabFootManager::create();
	crab_foot_manager->transform()->setParent(transform_);

	auto crab_arm_manager = CrabArmManager::create();
	crab_arm_manager->transform()->setParent(transform_);

	auto sphere_collider = std::make_shared<SphereCollider>(*this);
	sphere_collider->transform()->setPos(Vec3f(0, 1, 0));
	addComponent(sphere_collider);
}

std::shared_ptr<Item>const Item::create(const ci::Vec3f& pos, Type type)
{
	auto instance = std::make_shared<Item>(pos, type);
	GameObjectTask::getInstnace().add(instance);
	return instance;
}

void Item::changeDrainedState(std::shared_ptr<Player>& player)
{
	state_ = State::DRAINED;
	player_ = player;
}

void Item::start()
{
	auto sphere_collider = getComponent<SphereCollider>();
	auto rocks_itr = findObjects("Rock");
	for (auto itr = rocks_itr.first; itr != rocks_itr.second; ++itr)
	{
		sphere_collider->regist(itr->second->getComponent<BoxCollider>());
	}

	auto back_ground = findObject("Background");
	sphere_collider->regist(back_ground->getComponent<SphereCollider>());

	auto player_num = static_cast<int>(Scene::playerNum());
	for (auto i = 0; i < player_num; ++i)
	{
		auto player = GameObject::findObject("Player" + std::to_string(i + 1));
		if (nullptr == player)continue;
		auto player_collider = player->getComponent<BoxCollider>();
		sphere_collider->regist(player_collider);
	}

	auto player_drain_attack = findObjects("PlayerDrainAttack");
	for (auto itr = player_drain_attack.first; itr != player_drain_attack.second; ++itr)
	{
		sphere_collider->regist(itr->second->getComponent<SphereCollider>());
	}
}

void Item::update()
{
	inAppearance();
	inMove();
	inDrained();
	inInvincible();
}

void Item::draw()
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

	//gl::pushModelView();
	//gl::multModelView(getComponent<SphereCollider>()->debugMatrix());

	//gl::drawSphere(Vec3f::zero(), 1.0f);

	//gl::popModelView();
}


void Item::onCollisionEnter(GameObject& gameobject)
{
	if (gameobject.getHashCode() != std::hash<std::string>()("Rock"))return;
	onEnterInAppearance(gameobject);
	onEnterInMove(gameobject);
}

void Item::onCollisionExit(GameObject& gameobject)
{
	if (gameobject.getHashCode() != std::hash<std::string>()("Background"))return;
	transform_->translate(-speed_);

	if (type_ == Type::INVINCIBLE)
	{
		speed_ = Vec3f(Rand::randFloat(0.3, 0.4f), 0, Rand::randFloat(0.3f, 0.4f));

		if (Rand::randBool())
		{
			speed_.x *= -1;
		}

		if (Rand::randBool())
		{
			speed_.z *= -1;
		}
	}
	else
	{
		speed_ = Vec3f(Rand::randFloat(-SPEED_RAND, SPEED_RAND), 0, Rand::randFloat(-SPEED_RAND, SPEED_RAND));
	}

}

void Item::onEnterInAppearance(GameObject& gameobject)
{
	if (State::APPEARANCE != state_)return;

	destroy();
}

void Item::onEnterInMove(GameObject& gameobject)
{
	if (State::MOVE != state_)return;

	transform_->translate(-speed_);

	if (type_ == Type::INVINCIBLE)
	{
		speed_ = Vec3f(Rand::randFloat(0.3, 0.4f), 0, Rand::randFloat(0.3f, 0.4f));

		if (Rand::randBool())
		{
			speed_.x *= -1;
		}

		if (Rand::randBool())
		{
			speed_.z *= -1;
		}
	}
	else
	{
		speed_ = Vec3f(Rand::randFloat(-SPEED_RAND, SPEED_RAND), 0, Rand::randFloat(-SPEED_RAND, SPEED_RAND));
	}
}

void Item::inAppearance()
{
	if (State::APPEARANCE != state_)return;

	transform_->translate(Vec3f::yAxis() / 90);

	auto direction = speed_.safeNormalized();
	auto radian = std::sinf(appearance_count_) * M_PI / 10;
	transform_->setRotate(Quatf(Vec3f::zAxis(), direction)* Quatf(Vec3f::yAxis(), M_PI * 0.5f) * Quatf(Vec3f::zAxis(), radian));

	appearance_count_ += 0.10f;

	if (appearance_count_ <= 0.10f * 90)return;
	state_ = State::MOVE;
	appearance_count_ = 0.0f;
}

void Item::inMove()
{
	if (State::MOVE != state_)return;

	transform_->translate(speed_);

	auto direction = speed_.safeNormalized();
	transform_->setRotate(Quatf(Vec3f::zAxis(), direction)* Quatf(Vec3f::yAxis(), M_PI * 0.5f));
}

void Item::inDrained()
{
	if (State::DRAINED != state_)return;

	if (death_count_ == 0.0f)
	{
		timeline().apply(&transform_->getAnimScale(), Vec3f::zero(), 2.0f);
	}

	auto target = player_.lock()->transform()->worldPos() - transform_->worldPos();
	target.y += 1.5f;
	transform_->translate(target * death_count_ / 180);

	auto direction = speed_.safeNormalized();
	transform_->setRotate(Quatf(Rand::randVec3f(), Rand::randFloat(0.1f, 0.7f)));

	death_count_ += 1.0f;

	if (death_count_ <= 120)return;
	destroy();
}

void Item::inInvincible()
{
	if (Type::INVINCIBLE != type_)return;

	invincible_count_++;

	if (invincible_count_ % 5 != 0)return;

	material_.setDiffuse(ColorA(Rand::randFloat(0.0f, 1.0f), Rand::randFloat(0.0f, 1.0f), Rand::randFloat(0.0f, 1.0f)));
	material_.setAmbient(material_.getDiffuse() * 0.5f);
}