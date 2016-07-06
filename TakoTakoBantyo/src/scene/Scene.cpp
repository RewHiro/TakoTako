#include "Scene.h"

#include "input/UtilityGamePad.h"

#include "gameobject/GameObjectTask.h"

using namespace input;

Scene::Scene()
{
	GamePad::getInstance().init();
}

Scene::~Scene()
{
	GameObjectTask::getInstnace().clear();
}