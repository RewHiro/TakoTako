#include "PlayerBullet.h"

#include "ResourceManager.h"

#include "scene/Stage.h"

#include "GameObjectTask.h"
#include "Player.h"

#include "component/Transform.h"
#include "component/SphereCollider.h"
#include "component/BoxCollider.h"

using namespace ci;

PlayerBullet::PlayerBullet(const ci::Vec3f& pos, const ci::Vec3f& direction, const PlayerType type, const ci::Quatf& rotate, int player_num) :
GameObject("PlayerBullet" + std::to_string(player_num), pos - Vec3f::yAxis() * 0.5f, Vec3f::one() * 2, rotate),
direction_(direction),
type_(type)
{


	ResourceManager::getInstance().find(ModelType::PLAYER_BULLET).load(
		&mesh_,
		boost::logic::indeterminate,
		boost::logic::indeterminate,
		false
		);

	transform_->rotate(Quatf(Vec3f::yAxis(), 3.14f));

	save_rotate_ = transform_->getRotate();


	switch (type)
	{
		case PlayerType::ONE:

			material_.setDiffuse(ColorA(255 / 255.0f, 0 / 255.0f, 119 / 255.0f));
			material_.setAmbient(material_.getDiffuse()*0.5f);
			material_.setSpecular(ColorA(0.4f, 0.4f, 0.4f));

			break;
		case PlayerType::TWO:

			material_.setDiffuse(ColorA(224 / 255.0f, 234 / 255.0f, 58 / 255.0f));
			material_.setAmbient(material_.getDiffuse()*0.5f);
			material_.setSpecular(ColorA(0.4f, 0.4f, 0.4f));
			break;
		case PlayerType::THREE:

			material_.setDiffuse(ColorA(40 / 255.0f, 175 / 255.0f, 12 / 255.0f));
			material_.setAmbient(material_.getDiffuse()*0.5f);
			material_.setSpecular(ColorA(0.4f, 0.4f, 0.4f));
			break;
		case PlayerType::FOUR:

			material_.setDiffuse(ColorA(158 / 255.0f, 40 / 255.0f, 163 / 255.0f));
			material_.setAmbient(material_.getDiffuse()*0.5f);
			material_.setSpecular(ColorA(0.4f, 0.4f, 0.4f));

			break;
	}

	material_.setEmission(ColorA(0.3f, 0.3f, 0.3f));

	auto sphere_collider = std::make_shared<SphereCollider>(*this);
	sphere_collider->transform()->setScale(Vec3f::one()*0.4f);
	addComponent(sphere_collider);

	direction_ += Vec3f::yAxis() * 0.15f;
	direction_.safeNormalize();
}

std::shared_ptr<PlayerBullet>const PlayerBullet::create(const ci::Vec3f& pos, const ci::Vec3f& direction, const PlayerType type, const ci::Quatf& rotate, int player_num)
{
	auto instance = std::make_shared<PlayerBullet>(pos + Vec3f::yAxis() * 3.35f, direction, type, rotate, player_num);
	GameObjectTask::getInstnace().add(instance);
	return instance;
}

void PlayerBullet::start()
{
	auto sphere_collider = getComponent<SphereCollider>();
	sphere_collider->regist(GameObject::findObject("Background")->getComponent<SphereCollider>());

	auto player_num = static_cast<int>(Scene::playerNum());
	for (auto i = 0; i < player_num; ++i)
	{
		auto player_collider = GameObject::findObject("Player" + std::to_string(i + 1))->getComponent<BoxCollider>();
		sphere_collider->regist(player_collider);
	}

	auto rocks_itr = findObjects("Rock");
	for (auto itr = rocks_itr.first; itr != rocks_itr.second; ++itr)
	{
		sphere_collider->regist(itr->second->getComponent<BoxCollider>());
	}

}

void PlayerBullet::update()
{
	transform_->setRotate(Quatf(Vec3f::zAxis(), Vec3f(0,-direction_.y,1).safeNormalized()) * save_rotate_);
	transform_->translate(direction_ * 1.f);

	direction_.y += -0.01f;

	if (transform_->worldPos().y < -0.5f)
	{
		destroy();
	}
}

void PlayerBullet::draw()
{
	gl::pushModelView();

	gl::multModelView(transform_->worldMatrix());
	material_.apply();

	gl::draw(mesh_);

	gl::popModelView();

	//#ifdef DEBUG
	//	gl::pushModelView();
	//
	//	gl::multModelView(getComponent<SphereCollider>()->debugMatrix());
	//	material_.apply();
	//
	//	gl::drawSphere(Vec3f::zero(), 1.0f, 20);
	//
	//	gl::popModelView();
	//
	//#endif
}

void PlayerBullet::onCollisionEnter(GameObject& gameobject)
{
	if (gameobject.getHashCode() == std::hash<std::string>()("Player" + std::to_string(static_cast<int>(type_))))return;
	if (gameobject.getHashCode() == std::hash<std::string>()("Background"))return;
	destroy();
}

void PlayerBullet::onCollisionExit(GameObject& gameobject)
{
	if (gameobject.getHashCode() != std::hash<std::string>()("Background"))return;
	destroy();
}