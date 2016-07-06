#pragma once
#include "Object.h"

#include "cinder/vector.h"
#include "cinder/Quaternion.h"

#include "Using.h"

#include "component/ComponentTask.h"

class GameObjectTask;
class Transform;

namespace object
{
	class Camera;
}

class GameObject : public Object, public std::enable_shared_from_this<GameObject>
{
public:

	GameObject();
	explicit GameObject(
		const std::string& name,
		const ci::Vec3f& pos = ci::Vec3f::zero(),
		const ci::Vec3f& scale = ci::Vec3f::one(),
		const ci::Quatf& rotate = ci::Quatf::identity(),
		int priority = 0);

	std::shared_ptr<Transform> const transform()const{ return transform_; }

	static std::shared_ptr<GameObject>const findObject(const std::string& name);
	static std::pair<GAMEOBJ_MAP_ITR, GAMEOBJ_MAP_ITR>& findObjects(const std::string& name);

	template<class T>
	std::shared_ptr<T> getObject()
	{
		return std::dynamic_pointer_cast<T>(shared_from_this());
	}

	std::shared_ptr<Component>const findComponent(const std::string& name);

	template<class T>
	std::shared_ptr<T> getComponent()
	{
		const std::string& name = typeid(T).name();
		return std::dynamic_pointer_cast<T>(findComponent(name.substr(6)));
	}

	void destroy()override;
	void onCollisionEnter(GameObject& gameobject)override{ component_task_.onCollisionEnter(gameobject); }
	void onCollisionExit(GameObject& gameobject)override{ component_task_.onCollisionExit(gameobject); }
	void momentCollisionExit(GameObject& gameobject)override{ component_task_.momentCollisionExit(gameobject); }

protected:

	void start()override{}
	void update()override{}
	virtual void draw(){}
	virtual void draw(const std::weak_ptr<object::Camera>& camera){}
	virtual void resize(){}

	void addComponent(const std::shared_ptr<Component>& component);

	std::shared_ptr<Transform> const transform_;

private:

	friend class GameObjectTask;

	int priority_;

	ComponentTask component_task_;
};