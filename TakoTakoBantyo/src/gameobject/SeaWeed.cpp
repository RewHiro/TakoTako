#include "SeaWeed.h"

#include "GameObjectTask.h"
#include "Camera.h"

#include "component/Transform.h"

#include "ResourceManager.h"

using namespace ci;

SeaWeed::SeaWeed(const ci::Vec3f& pos, const ci::Vec3f& scale, bool is_title) :
GameObject("SeaWeed", pos, scale),
texture_(ResourceManager::getInstance().find(TextureType::SEAWEED)),
is_title_(is_title)
{
	material_.setAmbient(ColorA::white());
	material_.setDiffuse(ColorA::white());
	material_.setEmission(ColorA::white());
}

std::shared_ptr<SeaWeed>const SeaWeed::create(const ci::Vec3f& pos, const ci::Vec3f& scale, bool is_title)
{
	auto instance = std::make_shared<SeaWeed>(pos, scale, is_title);
	GameObjectTask::getInstnace().add(instance, true);
	return instance;
}

void SeaWeed::update()
{
	angle_ += 0.1f;
}

void SeaWeed::draw(const std::weak_ptr<object::Camera>& camera)
{
	gl::enable(GL_TEXTURE_2D);
	gl::disable(GL_CULL_FACE);

	gl::pushModelView();

	texture_.bind();
	Vec3f right, up;
	camera.lock()->getCameraPersp().getBillboardVectors(&right, &up);
	material_.apply();

	gl::drawBillboard(transform_->getPos(), transform_->getScale().xy(), 0.0f, right, Vec3f::yAxis());

	texture_.unbind();

	gl::popModelView();

	gl::enable(GL_CULL_FACE);
	gl::disable(GL_TEXTURE_2D);
}