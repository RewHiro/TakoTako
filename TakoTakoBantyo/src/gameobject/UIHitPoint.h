#pragma once

#include "GameObject.h"

class Player;

class UIHitPoint : public GameObject
{

public:

	explicit UIHitPoint(int num, const std::shared_ptr<Player>& player);
	static std::shared_ptr<UIHitPoint>const create(int num, const std::shared_ptr<Player>& player);

	void death();

private:

	void resize()final;
	void draw()final;

	const ci::gl::Texture& texture_;
	int num_;
};