#pragma once 

#include "Object.h"

#include "gameobject/GameObject.h"

class ComponentTask;

class Component : public Object
{
public:

	explicit Component(GameObject& game_object);

	GameObject& gameObject()const{ return game_object_; }

	void start()override{}
	void update()override{}

protected:

private:

	friend class ComponentTask;

	virtual std::string typeName()const;

	GameObject& game_object_;
};