#include "Rock.h"

#include "cinder/Rand.h"

#include "ResourceManager.h"

#include "GameObjectTask.h"
#include "BubbleManager.h"

#include "component/Transform.h"
#include "component/BoxCollider.h"

using namespace ci;

Rock::Rock(const ci::Vec3f& pos, const ci::Vec3f& scale, const ci::Quatf& rotate, const RockType rock_type) :
GameObject("Rock", pos, scale, rotate),
create_count_(Rand::randInt(60 * 2, 60 * 4))
{
	auto box_collider = std::make_shared<BoxCollider>(*this);
	switch (rock_type)
	{
		case RockType::ROCK01:
			texture_ = ResourceManager::getInstance().find(TextureType::ROCK01);
			ResourceManager::getInstance().find(ModelType::ROCK01).load(
				&mesh_,
				boost::logic::indeterminate,
				boost::logic::indeterminate,
				false
				);
			box_collider->transform()->setPos(Vec3f(0, 3, 0));
			box_collider->transform()->setScale(Vec3f(12, 10, 9));
			break;
		case RockType::ROCK02:
			texture_ = ResourceManager::getInstance().find(TextureType::ROCK02);
			ResourceManager::getInstance().find(ModelType::ROCK02).load(
				&mesh_,
				boost::logic::indeterminate,
				boost::logic::indeterminate,
				false
				);
			box_collider->transform()->setPos(Vec3f(0, 3, 0));
			box_collider->transform()->setScale(Vec3f(8, 5, 6));
			break;
	}

	material_.setDiffuse(ColorA(0.8f, 0.8f, 0.8f));
	material_.setAmbient(material_.getDiffuse()*0.5f);
	material_.setSpecular(ColorA(0.4f, 0.4f, 0.4f));
	addComponent(box_collider);
}

std::shared_ptr<Rock>const Rock::create(const ci::Vec3f& pos, const ci::Vec3f& scale, const ci::Quatf& rotate, const RockType rock_type)
{
	auto instance = std::make_shared<Rock>(pos, scale, rotate, rock_type);
	GameObjectTask::getInstnace().add(instance);
	return instance;
}

void Rock::update()
{

	if(create_count_ == 60 * 12)
	{
		create_count_ = Rand::randInt(60 * 2, 60 * 4);
		BubbleManager::create(transform_->worldPos());
	}
	create_count_++;
}

void Rock::draw()
{
	gl::enable(GL_TEXTURE_2D);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	gl::pushModelView();

	gl::multModelView(transform_->worldMatrix());

	material_.apply();
	texture_.bind();
	gl::draw(mesh_);

	texture_.unbind();


	gl::popModelView();

	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	gl::disable(GL_TEXTURE_2D);

#ifdef DEBUG

	gl::pushModelView();
	gl::multModelView(getComponent<BoxCollider>()->debugMatrix());

	gl::drawStrokedCube(Vec3f::zero(), Vec3f::one());

	gl::popModelView();

#endif
}