#pragma once

#include "GameObject.h"

enum class RockType
{
	ROCK01,
	ROCK02,
};

class Rock : public GameObject
{

public:

	Rock(const ci::Vec3f& pos, const ci::Vec3f& scale, const ci::Quatf& rotate, const RockType rock_type);
	static std::shared_ptr<Rock>const create(const ci::Vec3f& pos, const ci::Vec3f& scale, const ci::Quatf& rotate, const RockType rock_type);

private:

	void update()final;
	void draw()final;

	ci::gl::Texture texture_;
	ci::gl::Material material_;
	ci::TriMesh mesh_;
	int create_count_;
};