#include "ComponentTask.h"

#include "Component.h"

void ComponentTask::add(const ComponentSP& component)
{
	component->name_ = component->typeName();
	component_map_.emplace(component->name_, component);
}

void ComponentTask::update()
{
	for (auto& component : component_map_)
	{
		if (component.second->isUpdate())
		{
			component.second->update();
		}
		else
		{
			component.second->start();
			component.second->changeUpdate();
		}
	}
	destory();
}

ComponentSP ComponentTask::find(const std::string& name)
{
	return component_map_.find(name)->second;
}

void ComponentTask::onCollisionEnter(GameObject& gameobject)
{
	for(auto& component : component_map_)
	{
		component.second->onCollisionEnter(gameobject);
	}
}

void ComponentTask::onCollisionExit(GameObject& gameobject)
{
	for (auto& component : component_map_)
	{
		component.second->onCollisionExit(gameobject);
	}
}

void ComponentTask::momentCollisionExit(GameObject& gameobject)
{
	for (auto& component : component_map_)
	{
		component.second->momentCollisionExit(gameobject);
	}
}

void ComponentTask::destory()
{
	auto itr = component_map_.cbegin();
	while (itr != component_map_.cend())
	{
		if (itr->second->isDelete())
		{
			itr = component_map_.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}