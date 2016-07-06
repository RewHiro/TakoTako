#pragma once

#include "GameObject.h"

class PlayerLeftArm : public GameObject
{
public:

	PlayerLeftArm(const ci::TriMesh& mesh);
	static std::shared_ptr<PlayerLeftArm>const create(const ci::TriMesh& mesh);

private:

	void start()final;
	void draw()final;

	const ci::gl::Material* material_;
	const ci::TriMesh mesh_;
	ci::gl::Texture texture_;
};