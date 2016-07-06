#pragma once

#include "GameObject.h"

class Player;
class Item;
enum class PlayerType;

class PlayerDrainAttack : public GameObject
{
public:

	PlayerDrainAttack();
	static std::shared_ptr<PlayerDrainAttack>const create();

private:

	void start()final;
	void draw()final;

	void onCollisionEnter(GameObject& gameobject)final;
	void onCollisionExit(GameObject& gameobject)final;

	void onItemBulletEnter(std::weak_ptr<Item> item);
	void onItemInvincibleEnter(std::weak_ptr<Item> item);


	PlayerType player_type_;
	std::weak_ptr<Player>player_;
};