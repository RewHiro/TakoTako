#pragma once

#include "GameObject.h"

class Player;

class Item : public GameObject
{
public:

	enum class Type
	{
		INVINCIBLE,
		BULLET,
		SHOTGUN,
	};

	Item(const ci::Vec3f& pos, Type type);
	static std::shared_ptr<Item>const create(const ci::Vec3f& pos, Type type);

	const ci::gl::Material& getMaterial()const{ return material_; }
	ci::Vec3f getSpeed()const{ return speed_; }

	void changeDrainedState(std::shared_ptr<Player>& player);

	bool isDrained()const{ return State::DRAINED == state_; }
	bool isMove()const{ return State::MOVE == state_; }

	bool isBullet()const{ return Type::BULLET == type_; }
	bool isInvincible()const{ return Type::INVINCIBLE == type_; }

private:

	enum class State
	{
		APPEARANCE,
		MOVE,
		DRAINED,
	};

	static const float SPEED_RAND;

	void start()final;
	void update()final;
	void draw()final;

	void onCollisionEnter(GameObject& gameobject)final;
	void onCollisionExit(GameObject& gameobject)final;

	void onEnterInAppearance(GameObject& gameobject);
	void onEnterInMove(GameObject& gameobject);

	void inAppearance();
	void inMove();
	void inDrained();
	void inInvincible();

	std::weak_ptr<Player>player_;
	ci::TriMesh mesh_;
	ci::gl::Texture texture_;
	ci::gl::Material material_;
	ci::Vec3f speed_;
	float appearance_count_;
	float death_count_;
	int invincible_count_;
	State state_;
	Type type_;
};