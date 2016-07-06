#include "UIResultString.h"

#include "ResourceManager.h"

#include "GameObjectTask.h"
#include "Player.h"

#include "component/Transform.h"

using namespace ci;
using namespace ci::app;

UIResultString::UIResultString(const std::string& result_string) :
GameObject("UIResultString", Vec3f(0, getWindowHeight(), 0), Vec3f(2, -2, 1), Quatf(Vec3f::zAxis(), 0.2f)),
font_(ResourceManager::getInstance().find(FontType::MEIRYO_BOLD)),
result_string_(result_string)
{
	if (std::hash<std::string>()(result_string) == std::hash<std::string>()("Lose"))
	{
		color_ = ColorA(0, 0, 1);
	}
	else
	{
		color_ = ColorA(1, 0, 0);
	}
	timeline().apply(&transform_->getAnimPos(), Vec3f::zero(), 3.0f, EaseOutBounce());
}

std::shared_ptr<UIResultString>const UIResultString::create(const std::shared_ptr<Player>& player, const std::string& result_string)
{
	auto instance = std::make_shared<UIResultString>(result_string);
	GameObjectTask::getInstnace().add(instance, false, true, static_cast<int>(player->getType()));
	return instance;
}

void UIResultString::draw()
{
	gl::enableAlphaBlending();
	gl::pushModelView();
	gl::multModelView(transform_->worldMatrix());
	gl::drawStringCentered(result_string_, Vec2f::zero(), color_, font_);
	gl::popModelView();
	gl::disableAlphaBlending();
}