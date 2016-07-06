#pragma once

#include "GameObject.h"

class Background : public GameObject
{
public:

	Background();
	static std::shared_ptr<Background>const create();

private:

	void draw()final;

	const ci::gl::Texture& texture_;
	ci::gl::Material material_;
};