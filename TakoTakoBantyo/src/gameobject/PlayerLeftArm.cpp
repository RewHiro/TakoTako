#include "PlayerLeftArm.h"

#include "ResourceManager.h"

#include "GameObjectTask.h"
#include "Player.h"

#include "component/Transform.h"

using namespace ci;

PlayerLeftArm::PlayerLeftArm(const ci::TriMesh& mesh) :
GameObject("PlayerLeftArm", Vec3f(-1, -3, 0)),
mesh_(mesh),
texture_(ResourceManager::getInstance().find(TextureType::PLAYER_FOOT))
{
}

std::shared_ptr<PlayerLeftArm>const PlayerLeftArm::create(const ci::TriMesh& mesh)
{
	auto instance = std::make_shared<PlayerLeftArm>(mesh);
	GameObjectTask::getInstnace().add(instance);
	return instance;
}

void PlayerLeftArm::start()
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

void PlayerLeftArm::draw()
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