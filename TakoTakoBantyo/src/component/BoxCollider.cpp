#include "BoxCollider.h"

#include "Transform.h"
#include "SphereCollider.h"

using namespace ci;

BoxCollider::BoxCollider(GameObject& gameobject) :
Collider(gameobject)
{

}


float BoxCollider::squareDistancePointAABB(const std::weak_ptr<SphereCollider> sphere_collider, const BoxCollider& box_colider)const
{
	auto square_distance = 0.0f;
	auto aabb_min = box_colider.transform()->getPos() + box_colider.gameObject().transform()->worldPos() -
		(box_colider.transform()->getScale() * box_colider.gameObject().transform()->getScale()) * 0.5f;

	auto aabb_max = box_colider.transform()->getPos() + box_colider.gameObject().transform()->worldPos() +
		(box_colider.transform()->getScale() * box_colider.gameObject().transform()->getScale()) * 0.5f;

	auto point = sphere_collider.lock()->transform()->getPos() + sphere_collider.lock()->gameObject().transform()->worldPos();

	for (auto i = 0; i < 3; ++i)
	{
		auto v = point[i];
		if (v < aabb_min[i])square_distance += (aabb_min[i] - v) * (aabb_min[i] - v);
		if (v > aabb_max[i])square_distance += (aabb_max[i] - v) * (aabb_max[i] - v);
	}

	return square_distance;
}

bool BoxCollider::onCollisionEnter(const std::weak_ptr<BoxCollider>& box_collider)const
{
	//auto a_pos = hit_transform_->getPos() + gameObject().transform()->worldPos();
	//auto b_pos = box_collider.lock()->hit_transform_->getPos() + box_collider.lock()->gameObject().transform()->worldPos();

	//auto a_scale = hit_transform_->getScale() * gameObject().transform()->getScale() * 0.5f;
	//auto b_scale = box_collider.lock()->hit_transform_->getScale() * box_collider.lock()->gameObject().transform()->getScale() * 0.5f;

	//if (std::fabs(a_pos.x - b_pos.x) >
	//	a_scale.x + b_scale.x)return false;

	//if (std::fabs(a_pos.y - b_pos.y) >
	//	a_scale.y + b_scale.y)return false;

	//if (std::fabs(a_pos.z - b_pos.z) >
	//	a_scale.z + b_scale.z)return false;

	//return true;

	auto hit = obb(box_collider);
	return hit;
}

bool BoxCollider::onCollisionEnter(const std::weak_ptr<SphereCollider>& sphere_collider)const
{
	auto square_distance = squareDistancePointAABB(sphere_collider, *this);
	auto r = sphere_collider.lock()->gameObject().transform()->getScale().x * sphere_collider.lock()->transform()->getScale().x;
	auto hit = (square_distance <= r * r);

	return hit;
}


bool BoxCollider::obb(const std::weak_ptr<BoxCollider>& box_collider)const
{
	float ra, rb;
	Matrix33f r, abs_r;

	std::vector<Vec3f>a_axis;
	a_axis.emplace_back(gameObject().transform()->getRotate() * Vec3f::xAxis());
	a_axis.emplace_back(gameObject().transform()->getRotate() * Vec3f::yAxis());
	a_axis.emplace_back(gameObject().transform()->getRotate() * Vec3f::zAxis());

	std::vector<Vec3f>b_axis;
	b_axis.emplace_back(box_collider.lock()->gameObject().transform()->getRotate() * Vec3f::xAxis());
	b_axis.emplace_back(box_collider.lock()->gameObject().transform()->getRotate() * Vec3f::yAxis());
	b_axis.emplace_back(box_collider.lock()->gameObject().transform()->getRotate() * Vec3f::zAxis());

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			r.at(i, j) = a_axis[i].dot(b_axis[j]);
		}
	}

	Vec3f a_pos = gameObject().transform()->worldPos() +
		transform()->getPos();

	Vec3f b_pos = box_collider.lock()->gameObject().transform()->worldPos() +
		box_collider.lock()->transform()->getPos();


	Vec3f a_scale = gameObject().transform()->getScale() * transform()->getScale() * 0.5f;

	Vec3f b_scale = box_collider.lock()->gameObject().transform()->getScale() * box_collider.lock()->transform()->getScale() * 0.5f;


	Vec3f t = b_pos - a_pos;

	t = Vec3f(t.dot(a_axis[0]), t.dot(a_axis[1]), t.dot(a_axis[2]));

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			abs_r.at(i, j) = std::fabs(r.at(i, j)) + EPSILON;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		ra = a_scale[i];
		rb = b_scale[0] * abs_r.at(i, 0) + b_scale[1] * abs_r.at(i, 1) + b_scale[2] * abs_r.at(i, 2);
		if (std::fabs(t[i]) > ra + rb)return false;
	}

	for (int i = 0; i < 3; ++i)
	{
		ra = a_scale[0] * abs_r.at(0, i) + a_scale[1] * abs_r.at(1, i) + a_scale[2] * abs_r.at(2, i);
		rb = b_scale[i];
		if (std::fabs(t[0] * r.at(0, i) + t[1] * r.at(1, i) + t[2] * r.at(2, i)) > ra + rb)return false;
	}

	ra = a_scale[1] * abs_r.at(2, 0) + a_scale[2] * abs_r.at(1, 0);
	rb = b_scale[1] * abs_r.at(0, 2) + b_scale[2] * abs_r.at(0, 1);
	if (std::fabs(t[2] * r.at(1, 0) - t[1] * r.at(2, 0)) > ra + rb)return false;

	ra = a_scale[1] * abs_r.at(2, 1) + a_scale[2] * abs_r.at(1, 1);
	rb = b_scale[0] * abs_r.at(0, 2) + b_scale[2] * abs_r.at(0, 0);
	if (std::fabs(t[2] * r.at(1, 1) - t[1] * r.at(2, 1)) > ra + rb)return false;

	ra = a_scale[1] * abs_r.at(2, 2) + a_scale[2] * abs_r.at(1, 2);
	rb = b_scale[0] * abs_r.at(0, 1) + b_scale[1] * abs_r.at(0, 0);
	if (std::fabs(t[2] * r.at(1, 2) - t[1] * r.at(2, 2)) > ra + rb)return false;


	ra = a_scale[0] * abs_r.at(2, 0) + a_scale[2] * abs_r.at(0, 0);
	rb = b_scale[1] * abs_r.at(1, 2) + b_scale[2] * abs_r.at(1, 1);
	if (std::fabs(t[0] * r.at(2, 0) - t[2] * r.at(0, 0)) > ra + rb)return false;

	ra = a_scale[0] * abs_r.at(2, 1) + a_scale[2] * abs_r.at(0, 1);
	rb = b_scale[0] * abs_r.at(1, 2) + b_scale[2] * abs_r.at(1, 0);
	if (std::fabs(t[0] * r.at(2, 1) - t[2] * r.at(0, 1)) > ra + rb)return false;

	ra = a_scale[0] * abs_r.at(2, 2) + a_scale[2] * abs_r.at(0, 2);
	rb = b_scale[0] * abs_r.at(1, 1) + b_scale[1] * abs_r.at(1, 0);
	if (std::fabs(t[0] * r.at(2, 2) - t[2] * r.at(0, 2)) > ra + rb)return false;


	ra = a_scale[0] * abs_r.at(1, 0) + a_scale[1] * abs_r.at(0, 0);
	rb = b_scale[1] * abs_r.at(2, 2) + b_scale[2] * abs_r.at(2, 1);
	if (std::fabs(t[1] * r.at(0, 0) - t[0] * r.at(1, 0)) > ra + rb)return false;

	ra = a_scale[0] * abs_r.at(1, 1) + a_scale[1] * abs_r.at(0, 1);
	rb = b_scale[0] * abs_r.at(2, 2) + b_scale[2] * abs_r.at(2, 0);
	if (std::fabs(t[1] * r.at(0, 1) - t[0] * r.at(1, 1)) > ra + rb)return false;

	ra = a_scale[0] * abs_r.at(1, 2) + a_scale[1] * abs_r.at(0, 2);
	rb = b_scale[0] * abs_r.at(2, 1) + b_scale[1] * abs_r.at(2, 0);
	if (std::fabs(t[1] * r.at(0, 2) - t[0] * r.at(1, 2)) > ra + rb)return false;


	return true;
}