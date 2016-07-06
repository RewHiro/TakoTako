#pragma once

#include "GameObject.h"

class Bubble : public GameObject
{
public:

	Bubble(
		const ci::Vec3f& pos, 
		const ci::Vec3f& scale,
		float swing,
		float speed,
		bool is_title = false
		);

	static std::shared_ptr<Bubble>const create(
		const ci::Vec3f& pos,
		const ci::Vec3f& scale,
		float swing,
		float speed,
		bool is_title = false
		);

private:

	void update()final;
	void draw(const std::weak_ptr<object::Camera>& camera)final;

	ci::gl::Material material_;
	ci::gl::Texture texture_;
	ci::Vec3f center_point_;
	ci::Vec3f direction_;
	float swing_;
	float speed_;
	float count_;
	bool is_title_;
};