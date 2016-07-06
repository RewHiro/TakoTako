#include "Bubble.h"

#include "cinder/Rand.h"

#include "ResourceManager.h"

#include "Camera.h"
#include "GameObjectTask.h"

#include "component/Transform.h"

using namespace ci;

Bubble::Bubble(
	const ci::Vec3f& pos,
	const ci::Vec3f& scale,
	float swing,
	float speed,
	bool is_title
	) :
	GameObject("Bubble", pos, scale),
	texture_(ResourceManager::getInstance().find(TextureType::BUBBLE)),
	center_point_(pos),
	direction_(Vec3f::zero()),
	swing_(swing),
	speed_(speed),
	count_(0.0f),
	is_title_(is_title)
{
	material_.setAmbient(ColorA::white());
	material_.setDiffuse(ColorA::white());
	material_.setEmission(ColorA::white());

	std::mt19937 random_seed;
	random_seed.seed(std::random_device()());

	direction_ = Vec3f(
		std::uniform_real_distribution<float>(-1.0f, 1.0f)(random_seed), 
		std::uniform_real_distribution<float>(.0f, 1.0f)(random_seed), 
		std::uniform_real_distribution<float>(-1.0f, 1.0f)(random_seed));

	direction_.safeNormalize();
}

std::shared_ptr<Bubble>const Bubble::create(
	const ci::Vec3f& pos,
	const ci::Vec3f& scale,
	float swing,
	float speed,
	bool is_title
	)
{
	auto instance = std::make_shared<Bubble>(pos, scale, swing, speed, is_title);
	GameObjectTask::getInstnace().add(instance, true);
	return instance;
}

void Bubble::update()
{
	if(!is_title_)
	{
		auto x = std::sinf(count_) * swing_;

		transform_->setPos(Vec3f(center_point_.x + x, transform_->worldPos().y, transform_->worldPos().z));
		transform_->translate(Vec3f::yAxis() * speed_);

		transform_->scale(Vec3f::one() * 0.01f);

		count_ += 0.03f;

	}
	else
	{
		transform_->translate(direction_ * speed_);
		transform_->scale(Vec3f::one() * 0.01f);
	}
	if (transform_->worldPos().y <= 50.0f)return;
	destroy();
}

void Bubble::draw(const std::weak_ptr<object::Camera>& camera)
{
	gl::enable(GL_TEXTURE_2D);
	gl::disable(GL_CULL_FACE);

	gl::pushModelView();

	texture_.bind();
	Vec3f right, up;
	camera.lock()->getCameraPersp().getBillboardVectors(&right, &up);
	material_.apply();
	gl::drawBillboard(transform_->getPos(), transform_->getScale().xy(), 0.0f, right, up);
	texture_.unbind();

	gl::popModelView();

	gl::enable(GL_CULL_FACE);
	gl::disable(GL_TEXTURE_2D);
}