#pragma once

#include "GameObject.h"

class DamageEffect : public GameObject
{
public:

	DamageEffect(
		const ci::Vec3f& pos,
		const ci::Vec3f& scale,
		const ci::Vec3f& direction,
		const ci::gl::Material& material,
		const ci::gl::Texture& texture,
		float death_time,
		float torque,
		float speed
		);

	static std::shared_ptr<DamageEffect>const create(
		const ci::Vec3f& pos,
		const ci::Vec3f& scale,
		const ci::Vec3f& direction,
		const ci::gl::Material& material,
		const ci::gl::Texture& texture,
		float death_time,
		float torque,
		float speed
		);

private:
	const float GRAVITY_POWER;
	const float DEATH_TIME;
	
	void update()final;
	void draw(const std::weak_ptr<object::Camera>& camera)final;

	void gravity();

	ci::gl::Texture texture_;
	ci::gl::Material material_;
	ci::Vec3f direction_;
	float speed_;
	float angle_;
	float torque_;
	float life_time_;
	float alpha_;
	
};