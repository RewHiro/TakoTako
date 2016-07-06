#pragma once

#include "Collider.h"

class SphereCollider : public Collider
{
public:

	SphereCollider(GameObject& gameobject);

private:

	float squareDistancePointAABB(const SphereCollider& sphere_collider, const std::weak_ptr<BoxCollider> box_colider)const;
	bool onCollisionEnter(const std::weak_ptr<BoxCollider>& box_collider)const final;
	bool onCollisionEnter(const std::weak_ptr<SphereCollider>& sphere_collider)const final;

};