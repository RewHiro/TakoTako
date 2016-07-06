#pragma once

#include "GameObject.h"

class PlayerRightArm : public GameObject
{
public:

	PlayerRightArm(const ci::TriMesh& mesh);
	static std::shared_ptr<PlayerRightArm>const create(const ci::TriMesh& mesh);

private:

	void start()final;
	void draw()final;

	const ci::gl::Material* material_;
	const ci::TriMesh mesh_;
	ci::gl::Texture texture_;
};