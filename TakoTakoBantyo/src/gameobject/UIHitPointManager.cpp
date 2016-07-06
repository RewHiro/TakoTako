#include "UIHitPointManager.h"

#include "GameObjectTask.h"
#include "Player.h"
#include "UIHitPoint.h"

#include "component/Transform.h"

using namespace ci;
using namespace ci::app;

UIHitPointManager::UIHitPointManager(const std::shared_ptr<Player>& player) :
GameObject("UIHitPointManager"),
player_(player)
{
	for (auto i = 0; i < player_.lock()->getHP(); ++i)
	{
		auto ui_hit_point = UIHitPoint::create(i + 1, player);
		ui_hit_point->transform()->setParent(transform_);
	}
	player_hp_ = player->getHP();
}


std::shared_ptr<UIHitPointManager>const UIHitPointManager::create(const std::shared_ptr<Player>& player)
{
	auto insatnce = std::make_shared<UIHitPointManager>(player);
	GameObjectTask::getInstnace().add(insatnce, false, true, static_cast<int>(player->getType()));
	return insatnce;
}

void UIHitPointManager::start()
{
	player_hp_ = player_.lock()->getHP();
}

void UIHitPointManager::update()
{
	auto player_hp = player_.lock()->getHP();
	if (player_hp == player_hp_)return;

	if (player_hp < player_hp_)
	{
		reductionHP(player_hp);
	}
	else
	{
		increaseHP(player_hp);
	}

	player_hp_ = player_hp;
}

void UIHitPointManager::increaseHP(int player_hp)
{
	if (player_hp < player_hp_)return;

	for (auto i = player_hp_; i < player_hp; ++i)
	{
		auto ui_hit_point = UIHitPoint::create(i + 1, player_.lock());
		ui_hit_point->transform()->setParent(transform_);
	}
}

void UIHitPointManager::reductionHP(int player_hp)
{
	if (player_hp > player_hp_)return;

	for (auto& child : transform_->getChildren())
	{
		std::string ui_str = "UIHitPoint" + std::to_string(player_hp_);
		if (child->gameObject().getHashCode() != std::hash<std::string>()(ui_str))continue;
		child->gameObject().getObject<UIHitPoint>()->death();
		break;
	}
}