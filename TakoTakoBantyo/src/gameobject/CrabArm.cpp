#include "CrabArm.h"

#include "ResourceManager.h"

#include "GameObjectTask.h"
#include "Item.h"

#include "component/Transform.h"

using namespace ci;

CrabArm::CrabArm(const ci::Vec3f& pos, const ci::TriMesh& mesh):
GameObject("CrabArm",pos),
mesh_(mesh),
texture_(ResourceManager::getInstance().find(TextureType::ITEM01_ARM))
{

}

std::shared_ptr<CrabArm>const CrabArm::create(const ci::Vec3f& pos, const ci::TriMesh& mesh)
{
	auto instance = std::make_shared<CrabArm>(pos, mesh);
	GameObjectTask::getInstnace().add(instance);
	return instance;
}

void CrabArm::start()
{
	material_ = &transform()->getTopOfLevelParent()->gameObject().getObject<Item>()->getMaterial();
}

void CrabArm::draw()
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