#pragma once

#include <list>

#include "Component.h"

class Transform;
class BoxCollider;
class SphereCollider;

class Collider : public Component
{

public:

	Collider(GameObject& gameobject);

	void regist(std::weak_ptr<BoxCollider> collider);
	void regist(std::weak_ptr<SphereCollider> collider);

	std::shared_ptr<Transform> const transform()const{ return hit_transform_; }

	ci::Matrix44f debugMatrix()const;
	ci::Vec3f debugPos()const;
	ci::Vec3f debugScale()const;

protected:

	void start()override{}
	void update()final;

	virtual bool onCollisionEnter(const std::weak_ptr<BoxCollider>& box_collider)const = 0;
	virtual bool onCollisionEnter(const std::weak_ptr<SphereCollider>& sphere_collider)const = 0;


	std::shared_ptr<Transform> const hit_transform_;
	std::list<std::weak_ptr<BoxCollider>>box_colliders_;
	std::list<std::weak_ptr<SphereCollider>>sphere_colliders_;

	std::vector<bool>is_hit_box_colliders_;
	std::vector<bool>is_hit_sphere_colliders_;

	bool is_hit_enter_;

private:
};