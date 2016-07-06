#pragma once

#include "Component.h"

#include "cinder/vector.h"
#include "cinder/Quaternion.h"
#include "cinder/Timeline.h"

class Transform;
using TransformSP = std::shared_ptr<Transform>;

class Transform : public Component, public std::enable_shared_from_this < Transform >
{
public:
	Transform(GameObject& game_object);
	
	ci::Vec3f getPos()const{ return pos_; }
	ci::Vec3f getScale()const{ return scale_; }
	ci::Anim<ci::Vec3f>& getAnimPos(){ return pos_; }
	ci::Anim<ci::Vec3f>& getAnimScale(){ return scale_; }
	ci::Quatf getRotate()const{ return rotate_; }
	std::list<TransformSP>& getChildren(){ return chilidren_list_; }

	void setPos(const ci::Vec3f& pos){ pos_ = pos; }
	void setScale(const ci::Vec3f& scale){ scale_ = scale; }
	void setRotate(const ci::Quatf& rotate){ rotate_ = rotate; }

	void translate(const ci::Vec3f& pos){ pos_.value() += pos; }
	void scale(const ci::Vec3f& scale){ scale_.value() += scale; }
	void rotate(const ci::Quatf& rotate){ rotate_ *= rotate; }

	void setParent(const TransformSP& transform);
	void resetParent();
	TransformSP getParent()const{ return parent_; }
	TransformSP getTopOfLevelParent();

	int childCount()const{ return chilidren_list_.size(); }

	ci::Matrix44f localMatrix()const;
	ci::Matrix44f worldMatrix()const;
	ci::Vec3f worldPos()const{ return ci::Vec3f(worldMatrix().m03, worldMatrix().m13, worldMatrix().m23); }

	void setProjectionMatrix(ci::Matrix44f& matrix){ projection_matrix_ = matrix; }
	ci::Vec3f getProjectionPos()const{ return ci::Vec3f(projection_matrix_.m03, projection_matrix_.m13, projection_matrix_.m23); }

	void destoryer();

private:

	std::list<TransformSP>chilidren_list_;

	ci::Anim<ci::Vec3f>pos_;
	ci::Anim<ci::Vec3f>scale_;
	ci::Quatf rotate_;

	TransformSP parent_;

	ci::Matrix44f projection_matrix_;
};

