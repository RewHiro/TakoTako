#include "ItemManager.h"

#include "cinder/Rand.h"

#include "Item.h"
#include "GameObjectTask.h"

#include "component/Transform.h"

using namespace ci;

ItemManager::ItemManager():
GameObject("ItemManager"),
create_count_(0)
{
	random_.seed(std::random_device()());

	for (auto i = 0; i < 10; ++i)
	{
		setCreate();
	}
}

std::shared_ptr<ItemManager>const ItemManager::create()
{
	auto instance = std::make_shared<ItemManager>();
	GameObjectTask::getInstnace().add(instance);
	return instance;
}

void ItemManager::update()
{

	auto items = findObjects("Item");

	auto i = 0;
	for (auto item_itr = items.first; item_itr != items.second; ++item_itr)
	{
		++i;
	}

	if (i >= 20)return;

	if(create_count_ % (60 * 7) == 0)
	{
		setCreate();
	}
	create_count_++;
}

void ItemManager::setCreate()
{
	auto pos = Rand::randVec3f();
	pos.y = 0.0f;
	pos *= 70.0f;

	std::bernoulli_distribution random(0.1);
	if (random(random_))
	{
		auto item = Item::create(pos, Item::Type::INVINCIBLE);
	}
	else
	{
		auto item = Item::create(pos, Item::Type::BULLET);
	}
}