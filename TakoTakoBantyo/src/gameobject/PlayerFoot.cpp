#include "PlayerFoot.h"

#include "ResourceManager.h"

#include "component/Transform.h"

#include "GameObjectTask.h"
#include "Player.h"

using namespace ci;
using namespace ci::app;

PlayerFoot::PlayerFoot(int num, const ci::TriMesh& mesh, const ci::Vec3f& pos) :
GameObject("PlayerFoot" + std::to_string(num), pos),
mesh_(mesh),
texture_(ResourceManager::getInstance().find(TextureType::PLAYER_FOOT)),
is_drained_(false),
count_(0.0f)
{

}

std::shared_ptr<PlayerFoot>const PlayerFoot::create(int num, const ci::TriMesh& mesh, const ci::Vec3f& pos)
{
	auto instance = std::make_shared<PlayerFoot>(num, mesh, pos);
	GameObjectTask::getInstnace().add(instance);
	return instance;
}

void PlayerFoot::startDrained(const ci::Vec3f& drain_pos)
{
	timeline().apply(&transform()->getAnimPos(), Vec3f::zero(), 2.5f, EaseInCirc());
	is_drained_ = true;
}

void PlayerFoot::start()
{
	auto player = transform()->getTopOfLevelParent()->gameObject().getObject<Player>();
	material_ = &player->getMaterial();

	auto type = player->getType();
	switch (type)
	{
		case PlayerType::TWO:
			texture_ = ResourceManager::getInstance().find(TextureType::PLAYER02_FOOT);
			break;
		case PlayerType::THREE:
			texture_ = ResourceManager::getInstance().find(TextureType::PLAYER03_FOOT);
			break;
		case PlayerType::FOUR:
			texture_ = ResourceManager::getInstance().find(TextureType::PLAYER04_FOOT);
			break;
	}
}

void PlayerFoot::update()
{
	if (!is_drained_)return;
	count_ += 1.0f;

	if (count_ < 60 * 2.5f)return;
	destroy();
}

void PlayerFoot::draw()
{
	gl::enable(GL_TEXTURE_2D);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	gl::pushModelView();

	gl::multModelView(transform()->worldMatrix());
	material_->apply();
	texture_.bind();
	gl::draw(mesh_);
	texture_.unbind();

	gl::popModelView();

	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	gl::disable(GL_TEXTURE_2D);
}