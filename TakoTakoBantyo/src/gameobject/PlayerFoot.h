#pragma once

#include "GameObject.h"

class PlayerFoot : public GameObject
{
public:

	PlayerFoot(int num, const ci::TriMesh& mesh, const ci::Vec3f& pos);
	static std::shared_ptr<PlayerFoot>const create(int num, const ci::TriMesh& mesh, const ci::Vec3f& pos);

	void startDrained(const ci::Vec3f& drain_pos);

private:

	void start()final;
	void update()final;
	void draw()final;

	const ci::gl::Material* material_;
	const ci::TriMesh mesh_;
	ci::gl::Texture texture_;

	bool is_drained_;
	float count_;
	ci::Vec3f drain_pos_;
};