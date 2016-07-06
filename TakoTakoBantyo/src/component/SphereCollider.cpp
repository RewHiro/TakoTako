#include "SphereCollider.h"

#include "Transform.h"
#include "BoxCollider.h"

SphereCollider::SphereCollider(GameObject& gameobject) :
Collider(gameobject)
{

}

float SphereCollider::squareDistancePointAABB(const SphereCollider& sphere_collider, const std::weak_ptr<BoxCollider> box_colider)const
{
	auto square_distance = 0.0f;
	auto aabb_min = box_colider.lock()->transform()->getPos() + box_colider.lock()->gameObject().transform()->worldPos() -
		(box_colider.lock()->transform()->getScale() * box_colider.lock()->gameObject().transform()->getScale()) * 0.5f;

	auto aabb_max = box_colider.lock()->transform()->getPos() + box_colider.lock()->gameObject().transform()->worldPos() +
		(box_colider.lock()->transform()->getScale() * box_colider.lock()->gameObject().transform()->getScale()) * 0.5f;

	auto point = sphere_collider.transform()->getPos() + sphere_collider.gameObject().transform()->worldPos();

	for (auto i = 0; i < 3; ++i)
	{
		auto v = point[i];
		if (v < aabb_min[i])square_distance += (aabb_min[i] - v) * (aabb_min[i] - v);
		if (v > aabb_max[i])square_distance += (aabb_max[i] - v) * (aabb_max[i] - v);
	}

	return square_distance;
}

bool SphereCollider::onCollisionEnter(const std::weak_ptr<BoxCollider>& box_collider)const
{
	auto square_distance = squareDistancePointAABB(*this, box_collider);
	auto r = gameObject().transform()->getScale().x * transform()->getScale().x;
	auto hit = (square_distance <= r * r);

	return hit;
}

bool SphereCollider::onCollisionEnter(const std::weak_ptr<SphereCollider>& sphere_collider)const
{
	auto a_pos = gameObject().transform()->worldPos() + transform()->getPos();
	auto b_pos = sphere_collider.lock()->gameObject().transform()->worldPos() + sphere_collider.lock()->transform()->getPos();

	auto distance = a_pos - b_pos;
	auto distance2 = distance.dot(distance);

	auto a_radius = gameObject().transform()->getScale().x * transform()->getScale().x;
	auto b_radius = sphere_collider.lock()->gameObject().transform()->getScale().x * sphere_collider.lock()->transform()->getScale().x;

	auto radius_sum = a_radius + b_radius;

	auto hit = distance2 <= radius_sum * radius_sum;

	return hit;
}
