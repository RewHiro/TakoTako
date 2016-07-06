#include "CameraManager.h"

#include "GameObjectTask.h"

#include "component/CameraController.h"

using namespace ci;

CameraManager::CameraManager(const ci::Quatf& rotate) :
GameObject("CameraManager", Vec3f::zero(), Vec3f::one(), rotate)
{
	addComponent(std::make_shared<CameraController>(*this));
}

std::shared_ptr<CameraManager>const CameraManager::create(const ci::Quatf& rotate)
{
	auto instance = std::make_shared<CameraManager>(rotate);
	GameObjectTask::getInstnace().add(instance);
	return instance;
}