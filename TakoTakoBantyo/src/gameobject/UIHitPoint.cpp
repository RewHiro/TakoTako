#include "UIHitPoint.h"

#include "ResourceManager.h"

#include "GameObjectTask.h"
#include "Player.h"

#include "component/Transform.h"

using namespace ci;
using namespace ci::app;

UIHitPoint::UIHitPoint(int num, const std::shared_ptr<Player>& player) :
GameObject("UIHitPoint" + std::to_string(num),
Vec3f(
-getWindowWidth()*0.5f + getWindowWidth() * 0.05f + (num - 1) * getWindowWidth() * 0.05f,
getWindowHeight()*0.5f - getWindowHeight() * 0.05f,
0),
Vec3f::zero(),
Quatf(Vec3f::zAxis(), 3.14f)),
texture_(ResourceManager::getInstance().find(TextureType::UI_HIT_POINT)),
num_(num)
{
	timeline().apply(&transform_->getAnimScale(), Vec3f::one(), 1.0f, EaseOutBack());
}

std::shared_ptr<UIHitPoint>const UIHitPoint::create(int num, const std::shared_ptr<Player>& player)
{
	auto insatnce = std::make_shared<UIHitPoint>(num, player);
	GameObjectTask::getInstnace().add(insatnce, false, true, static_cast<int>(player->getType()));
	return insatnce;
}

void UIHitPoint::death()
{
	timeline().apply(&transform_->getAnimScale(), Vec3f::zero(), 1.0f, EaseInBack()).finishFn(std::bind(&UIHitPoint::destroy,this));
}

void UIHitPoint::resize()
{
	transform_->setPos(Vec3f(
		-getWindowWidth()*0.5f + getWindowWidth() * 0.05f + (num_ - 1) * getWindowWidth() * 0.05f,
		getWindowHeight()*0.5f - getWindowHeight() * 0.05f,
		0));
}

void UIHitPoint::draw()
{
	auto width = getWindowWidth() * 0.03f;
	auto height = getWindowHeight() * 0.05f;

	gl::enable(GL_TEXTURE_2D);
	gl::pushModelView();
	gl::multModelView(transform_->worldMatrix());
	texture_.bind();
	gl::drawSolidRect(Rectf(-width, -height, width, height));
	texture_.unbind();
	gl::popModelView();
	gl::disable(GL_TEXTURE_2D);
}