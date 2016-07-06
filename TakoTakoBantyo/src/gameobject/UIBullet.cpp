#include "UIBullet.h"

#include "ResourceManager.h"

#include "GameObjectTask.h"
#include "Player.h"
#include "component/PlayerShot.h"
#include "component/Transform.h"

using namespace ci;
using namespace ci::app;

UIBullet::UIBullet(const std::shared_ptr<Player>& player) :
GameObject("UIBullet",
Vec3f(
getWindowWidth()*0.5f - getWindowWidth() * 0.1f,
getWindowHeight()*0.5f - getWindowHeight() * 0.05f,
0
),
Vec3f(1, -1, 1)),
count_(100),
player_shot_(player->getComponent<PlayerShot>()),
font_(ResourceManager::getInstance().find(FontType::MEIRYO_BOLD))
{
	count_ = player_shot_.lock()->getBulletCount();
}

std::shared_ptr<UIBullet>const UIBullet::create(const std::shared_ptr<Player>& player)
{
	auto instance = std::make_shared<UIBullet>(player);
	GameObjectTask::getInstnace().add(instance, false, true, static_cast<int>(player->getType()));
	return instance;
}

void UIBullet::update()
{
	count_ = player_shot_.lock()->getBulletCount();
}

void UIBullet::resize()
{
	transform_->setPos(Vec3f(
		getWindowWidth()*0.5f - getWindowWidth() * 0.1f,
		getWindowHeight()*0.5f - getWindowHeight() * 0.05f,
		0
		));
}

void UIBullet::draw()
{
	gl::pushModelView();
	gl::multModelView(transform_->worldMatrix());
	gl::drawString(std::to_string(count_), Vec2f::zero(), ColorA::white(), font_);
	gl::popModelView();
}