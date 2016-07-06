#include "Component.h"

Component::Component(GameObject& game_object):
game_object_(game_object)
{

}

std::string Component::typeName()const
{
	const std::type_info& id = typeid(*this);
	const std::string& name = id.name();
	return name.substr(6);
}