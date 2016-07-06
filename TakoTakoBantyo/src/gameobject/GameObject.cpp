#include "GameObject.h"

#include "component/Transform.h"

#include "gameobject/GameObjectTask.h"

GameObject::GameObject() :
Object(),
priority_(0),
transform_(std::make_shared<Transform>(*this))
{

}

GameObject::GameObject(
	const std::string& name,
	const ci::Vec3f& pos,
	const ci::Vec3f& scale,
	const ci::Quatf& rotate,
	int priority):
	Object(name),
	priority_(priority),
	transform_(std::make_shared<Transform>(*this))
{
	transform()->setPos(pos);
	transform()->setScale(scale);
	transform()->setRotate(rotate);
}

std::shared_ptr<GameObject>const GameObject::findObject(const std::string& name)
{
	return GameObjectTask::getInstnace().find(name);
}

std::pair<GAMEOBJ_MAP_ITR, GAMEOBJ_MAP_ITR>& GameObject::findObjects(const std::string& name)
{
	return GameObjectTask::getInstnace().finds(name);
}

std::shared_ptr<Component>const GameObject::findComponent(const std::string& name)
{
	return component_task_.find(name);
}

void GameObject::destroy()
{
	is_delete_ = true;
	for (auto& child : transform_->getChildren())
	{
		child->gameObject().destroy();
	}
}

void GameObject::addComponent(const std::shared_ptr<Component>& component)
{
	component_task_.add(component);
}