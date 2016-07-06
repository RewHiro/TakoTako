#include "Background.h"

#include "cinder/gl/gl.h"

#include "ResourceManager.h"
#include "GameObjectTask.h"

#include "component/Transform.h"
#include "component/SphereCollider.h"

using namespace ci;

Background::Background() :
GameObject("Background", Vec3f::zero(), Vec3f::one() * 100),
texture_(ResourceManager::getInstance().find(TextureType::BACKGROUND))
{
	material_.setAmbient(ColorA::white());
	material_.setDiffuse(ColorA::white());
	material_.setEmission(ColorA::white());

	auto sphere_collider = std::make_shared<SphereCollider>(*this);
	sphere_collider->transform()->setScale(Vec3f::one() * 0.935f);
	addComponent(sphere_collider);
}

std::shared_ptr<Background>const Background::create()
{
	auto instance = std::make_shared<Background>();
	GameObjectTask::getInstnace().add(instance);
	return instance;
}

void Background::draw()
{
	gl::enable(GL_TEXTURE_2D);
	glFrontFace(GL_CW);

	gl::pushModelView();

	gl::multModelView(transform_->worldMatrix());

	material_.apply();
	texture_.bind();
	gl::drawSphere(Vec3f::zero(), 1.0f, 20);
	gl::drawSphere(Vec3f::zero(), 1.14f, 20);
	texture_.unbind();

	gl::popModelView();

	glFrontFace(GL_CCW);
	gl::disable(GL_TEXTURE_2D);

//#ifdef DEBUG
//
//	gl::pushModelView();
//	
//	gl::multModelView(getComponent<SphereCollider>()->debugMatrix());
//
//	gl::drawSphere(Vec3f::zero(), 1.0f, 20);
//
//	gl::popModelView();
//
//#endif
}