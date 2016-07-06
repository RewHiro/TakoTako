#include "UIManager.h"

#include "scene/Stage.h"

#include "GameObjectTask.h"
#include "UIHitPointManager.h"
#include "UIBullet.h"
#include "Player.h"
#include "UIResultBlind.h"
#include "UIResultString.h"

#include "component/Transform.h"

UIManager::UIManager(const std::shared_ptr<Player>& player) :
GameObject("UIManager"),
player_(player),
is_create_resutl_(false)
{
	auto ui_hit_point_manager = UIHitPointManager::create(player);
	ui_hit_point_manager->transform()->setParent(transform_);

	auto ui_bullet = UIBullet::create(player);
	ui_bullet->transform()->setParent(transform_);
}

void UIManager::update()
{
	if (is_create_resutl_)return;
	if (!player_.lock()->IsResult())return;

	auto ui_result_blind = UIResultBlind::create(player_.lock());
	ui_result_blind->transform()->setParent(transform_);

	if(player_.lock()->getState() == Player::State::DEATH)
	{
		auto ui_result_string = UIResultString::create(player_.lock(), "Lose");
		ui_result_string->transform()->setParent(transform_);
	}
	else
	{
		auto ui_result_string = UIResultString::create(player_.lock(), "Win!");
		ui_result_string->transform()->setParent(transform_);
	}

	Stage::getResultCount()++;

	is_create_resutl_ = true;
}

std::shared_ptr<UIManager>const UIManager::create(const std::shared_ptr<Player>& player)
{
	auto instance = std::make_shared<UIManager>(player);
	GameObjectTask::getInstnace().add(instance, false, true, static_cast<int>(player->getType()));
	return instance;
}