#pragma once

#include "GameObject.h"

enum class PlayerType;

class PlayerBullet : public GameObject
{
public:

	PlayerBullet(const ci::Vec3f& pos, const ci::Vec3f& direction, const PlayerType type, const ci::Quatf& rotate,int player_num);
	static std::shared_ptr<PlayerBullet>const create(const ci::Vec3f& pos, const ci::Vec3f& direction, const PlayerType type, const ci::Quatf& rotate, int player_num);

	ci::gl::Material getMaterial()const{ return material_; }

private:

	void start()final;
	void update()final;
	void draw()final;

	void onCollisionEnter(GameObject& gameobject)final;
	void onCollisionExit(GameObject& gameobject)final;

	PlayerType type_;
	ci::gl::Material material_;
	ci::Vec3f direction_;
	ci::Vec3f speed_;
	ci::TriMesh mesh_;
	ci::Quatf save_rotate_;
};