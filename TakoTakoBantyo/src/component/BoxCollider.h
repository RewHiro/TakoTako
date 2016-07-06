#pragma once

#include "Collider.h"

class BoxCollider : public Collider
{
public:

	BoxCollider(GameObject& gameobject);

private:

	float squareDistancePointAABB(const std::weak_ptr<SphereCollider> sphere_collider, const BoxCollider& box_colider)const;

	bool onCollisionEnter(const std::weak_ptr<BoxCollider>& box_collider)const final;
	bool onCollisionEnter(const std::weak_ptr<SphereCollider>& sphere_collider)const final;

	bool obb(const std::weak_ptr<BoxCollider>& box_collider)const;

private:
};