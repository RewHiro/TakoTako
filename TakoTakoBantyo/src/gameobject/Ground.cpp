#include "Ground.h"

#include "cinder/gl/gl.h"

#include "ResourceManager.h"
#include "GameObjectTask.h"

#include "component/Transform.h"

using namespace ci;

Ground::Ground(const ci::Vec3f& pos, const ci::Vec3f& scale) :
//GameObject("Ground", Vec3f(-960, 0, -540), Vec3f(0.09f, 0.3f, 1)/*Vec3f(200, 1, 200)*/, Quatf(Vec3f::xAxis(), 3.14f*0.5f)),
GameObject("Ground", pos - Vec3f::yAxis() * scale.y * 0.5f, scale/*Vec3f(200, 1, 200)*/),
texture_(ResourceManager::getInstance().find(TextureType::GROUND))
{
	//transform_->setPos(Vec3f(-texture_.getWidth()*0.5f * transform_->getScale().x, 0, -texture_.getHeight() *0.5f * transform_->getScale().y));

	material_.setDiffuse(ColorA::white());
	material_.setAmbient(ColorA::white());
	material_.setEmission(ColorA::white());
}

std::shared_ptr<Ground>const Ground::create(const ci::Vec3f& pos, const ci::Vec3f& scale)
{
	auto instance = std::make_shared<Ground>(pos, scale);
	GameObjectTask::getInstnace().add(instance);
	return instance;
}

void Ground::draw()
{

	gl::pushModelView();

	gl::multModelView(transform_->worldMatrix());

	gl::enable(GL_TEXTURE_2D);
	material_.apply();
	texture_.bind();
	//texture_.setCleanTexCoords(0.1f, 0.1f);
	//texture_.setMagFilter(GL_LINEAR);
	//texture_.setWrap(GL_CLAMP, GL_CLAMP);
	gl::drawCube(Vec3f::zero(), Vec3f::one());
	//gl::draw(texture_);
	texture_.unbind();

	gl::popModelView();

	gl::disable(GL_TEXTURE_2D);
}