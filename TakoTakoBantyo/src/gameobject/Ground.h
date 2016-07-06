#pragma once

#include "GameObject.h"

class Ground : public GameObject
{
public:

	Ground(const ci::Vec3f& pos, const ci::Vec3f& scale);
	static std::shared_ptr<Ground>const create(const ci::Vec3f& pos, const ci::Vec3f& scale);

private:

	void draw()final;

	ci::gl::Texture texture_;
	ci::gl::Material material_;
};