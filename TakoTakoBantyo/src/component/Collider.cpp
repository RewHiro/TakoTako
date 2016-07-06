#include "Collider.h"

#include "Transform.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

using namespace ci;

Collider::Collider(GameObject& gameobject) :
Component(gameobject),
hit_transform_(std::make_shared<Transform>(gameobject)),
is_hit_enter_(false)
{

}

void Collider::regist(std::weak_ptr<BoxCollider> collider)
{
	box_colliders_.emplace_back(collider);
	is_hit_box_colliders_.push_back(false);
}

void Collider::regist(std::weak_ptr<SphereCollider> collider)
{
	sphere_colliders_.emplace_back(collider);
	is_hit_sphere_colliders_.push_back(false);
}

ci::Matrix44f Collider::debugMatrix()const
{
	auto translate = Matrix44f::createTranslation(gameObject().transform()->worldPos() + hit_transform_->getPos());
	auto hit_translate = Matrix44f::createTranslation(hit_transform_->getPos());
	auto scale = Matrix44f::createScale(hit_transform_->getScale() * gameObject().transform()->getScale());
	auto rotate = Matrix44f::createRotation(gameObject().transform()->getRotate().getAxis(), gameObject().transform()->getRotate().getAngle());
	auto affine = translate * rotate * scale;
	return affine;
}

ci::Vec3f Collider::debugPos()const
{
	return ci::Vec3f(debugMatrix().m03, debugMatrix().m13, debugMatrix().m23);
}

ci::Vec3f Collider::debugScale()const
{
	return hit_transform_->getScale() * gameObject().transform()->getScale();
}

void Collider::update()
{
	auto i = 0;
	for (auto& box_collider : box_colliders_)
	{
		auto hit = onCollisionEnter(box_collider);
		if (hit)
		{
			gameObject().onCollisionEnter(box_collider.lock()->gameObject());
			box_collider.lock()->gameObject().onCollisionEnter(gameObject());
			box_collider.lock()->is_hit_enter_ = true;
			is_hit_box_colliders_[i] = true;
		}
		else
		{
			gameObject().onCollisionExit(box_collider.lock()->gameObject());
			box_collider.lock()->gameObject().onCollisionExit(gameObject());

			if (box_collider.lock()->is_hit_enter_ && is_hit_box_colliders_[i])
			{
				gameObject().momentCollisionExit(box_collider.lock()->gameObject());
				box_collider.lock()->gameObject().momentCollisionExit(gameObject());
				box_collider.lock()->is_hit_enter_ = false;
				is_hit_box_colliders_[i] = false;
			}
		}
		++i;
	}

	i = 0;
	for (auto& sphere_collider : sphere_colliders_)
	{
		auto hit = onCollisionEnter(sphere_collider);
		if (hit)
		{
			gameObject().onCollisionEnter(sphere_collider.lock()->gameObject());
			sphere_collider.lock()->gameObject().onCollisionEnter(gameObject());
			sphere_collider.lock()->is_hit_enter_ = true;
			is_hit_sphere_colliders_[i] = true;
		}
		else
		{
			gameObject().onCollisionExit(sphere_collider.lock()->gameObject());
			sphere_collider.lock()->gameObject().onCollisionExit(gameObject());

			if (sphere_collider.lock()->is_hit_enter_ && is_hit_sphere_colliders_[i])
			{
				gameObject().momentCollisionExit(sphere_collider.lock()->gameObject());
				sphere_collider.lock()->gameObject().momentCollisionExit(gameObject());
				sphere_collider.lock()->is_hit_enter_ = false;
				is_hit_sphere_colliders_[i] = false;

			}
		}
		++i;
	}
}