#include "Transform.h"

using namespace ci;


Transform::Transform(GameObject& game_object) :
Component(game_object),
pos_(Vec3f::zero()),
scale_(Vec3f::one()),
rotate_(Quatf::identity())
{
}

ci::Matrix44f Transform::localMatrix()const
{
	auto translate = Matrix44f::createTranslation(pos_);
	auto scale = Matrix44f::createScale(scale_);
	return translate * rotate_.toMatrix44() * scale;
}

ci::Matrix44f Transform::worldMatrix()const
{
	if (!parent_)
		return localMatrix();

	return parent_->worldMatrix() * localMatrix();
}

void Transform::setParent(const TransformSP& transform)
{
	parent_ = transform;
	transform->chilidren_list_.emplace_back(shared_from_this());
}

void Transform::resetParent()
{
	parent_ = nullptr;
}

TransformSP Transform::getTopOfLevelParent()
{
	if (!parent_)return shared_from_this();
	return getParent()->getTopOfLevelParent();
}

void Transform::destoryer()
{
	chilidren_list_.remove_if([](const std::weak_ptr<Transform const >& transform){return transform.lock()->gameObject().isDelete(); });
}