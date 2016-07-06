#pragma once

#include "GameObject.h"

namespace object
{
	class Camera;
}

class SeaWeed : public GameObject
{
public:

	SeaWeed(const ci::Vec3f& pos, const ci::Vec3f& scale, bool is_title = false);
	static std::shared_ptr<SeaWeed>const create(const ci::Vec3f& pos, const ci::Vec3f& scale, bool is_title = false);

private:

	void update()final;
	void draw(const std::weak_ptr<object::Camera>& camera)final;

	const ci::gl::Texture& texture_;
	ci::gl::Material material_;
	ci::Vec3f up_;
	float angle_;
	bool is_title_;
};