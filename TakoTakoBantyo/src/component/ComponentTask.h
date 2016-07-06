#pragma once

#include <memory>
#include <unordered_map>

#include "boost/noncopyable.hpp"

class Component;
class GameObject;
using ComponentSP = std::shared_ptr < Component >const ;


class ComponentTask : private boost::noncopyable
{
public:
	void add(const ComponentSP& component);
	void update();
	ComponentSP find(const std::string& name);

	void onCollisionEnter(GameObject& gameobject);
	void onCollisionExit(GameObject& gameobject);
	void momentCollisionExit(GameObject& gameobject);

private:
	void destory();
	std::unordered_map<std::string, ComponentSP>component_map_;
};