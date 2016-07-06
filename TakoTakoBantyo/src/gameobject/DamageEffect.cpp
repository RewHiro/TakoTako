#include "DamageEffect.h"

#include "GameObjectTask.h"
#include "Camera.h"

#include "component/Transform.h"

using namespace ci;

DamageEffect::DamageEffect(
	const ci::Vec3f& pos,
	const ci::Vec3f& scale,
	const ci::Vec3f& direction,
	const ci::gl::Material& material,
	const ci::gl::Texture& texture,
	float death_time,
	float torque,
	float speed
	) :
	GameObject("DamageEffect", pos, scale),
	DEATH_TIME(death_time),
	GRAVITY_POWER(0.01f),
	texture_(texture),
	material_(material),
	direction_(direction),
	speed_(speed),
	angle_(0.0f),
	torque_(torque),
	life_time_(0.0f),
	alpha_(1.0f)
{
	material_.setSpecular(ColorA(0.0f, 0.0f, 0.0f, 0.0f));
}

std::shared_ptr<DamageEffect>const DamageEffect::create(
	const ci::Vec3f& pos,
	const ci::Vec3f& scale,
	const ci::Vec3f& direction,
	const ci::gl::Material& material,
	const ci::gl::Texture& texture,
	float death_time,
	float torque,
	float speed
	)
{
	auto instance = std::make_shared<DamageEffect>(pos, scale, direction, material, texture, death_time, torque, speed);
	GameObjectTask::getInstnace().add(instance, true);
	return instance;
}

void DamageEffect::update()
{

	transform_->translate(direction_.safeNormalized() * speed_);
	angle_ += torque_;

	alpha_ = (DEATH_TIME * 60 - life_time_) / DEATH_TIME * 60;

	auto color = material_.getDiffuse();
	material_.setDiffuse(ColorA(color.r, color.g, color.b, alpha_));
	material_.setAmbient(ColorA(color.r, color.g, color.b, alpha_));
	material_.setEmission(ColorA(color.r, color.g, color.b, alpha_));

	life_time_ += 1.0f;

	gravity();
	if (DEATH_TIME * 60 >= life_time_)return;
	destroy();
}

void DamageEffect::draw(const std::weak_ptr<object::Camera>& camera)
{
	gl::enable(GL_TEXTURE_2D);
	gl::disable(GL_CULL_FACE);
	gl::enableAdditiveBlending();

	gl::pushModelView();

	texture_.bind();
	Vec3f right, up;
	camera.lock()->getCameraPersp().getBillboardVectors(&right, &up);
	material_.apply();
	gl::drawBillboard(transform_->getPos(), transform_->getScale().xy(), angle_, right, up);
	gl::drawBillboard(transform_->getPos(), transform_->getScale().xy(), angle_, right, up);
	texture_.unbind();

	gl::popModelView();

	gl::enableAlphaBlending(false);
	gl::enable(GL_CULL_FACE);
	gl::disable(GL_TEXTURE_2D);
}

void DamageEffect::gravity()
{
	direction_.y -= GRAVITY_POWER;
}