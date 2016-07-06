#include "UIResultBlind.h"

#include "GameObjectTask.h"
#include "Player.h"

#include "component/Transform.h"

using namespace ci;
using namespace ci::app;

UIResultBlind::UIResultBlind() :
GameObject("UIResultBlind")
{

}

std::shared_ptr<UIResultBlind>const UIResultBlind::create(std::shared_ptr<Player>const player)
{
	auto instance = std::make_shared<UIResultBlind>();
	GameObjectTask::getInstnace().add(instance, false, true, static_cast<int>(player->getType()));
	return instance;
}

void UIResultBlind::draw()
{
	gl::pushModelView();
	gl::multModelView(transform_->worldMatrix());
	gl::enableAlphaBlending();
	gl::color(ColorA(0, 0, 0, 0.5f));
	gl::drawSolidRect(Rectf(-getWindowWidth(), -getWindowHeight(), getWindowWidth(), getWindowHeight()));
	gl::disableAlphaBlending();
	gl::color(ColorA::white());
	gl::popModelView();
}