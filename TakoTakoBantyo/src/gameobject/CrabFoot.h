#pragma once

#include "GameObject.h"

class CrabFoot : public GameObject
{
public:
	CrabFoot(const ci::Vec3f& pos, const ci::TriMesh& mesh);
	static std::shared_ptr<CrabFoot>const create(const ci::Vec3f& pos, const ci::TriMesh& mesh);

private:

	const ci::gl::Material* material_;
	const ci::TriMesh mesh_;
	const ci::gl::Texture& texture_;

	void start()final;
	void draw()final;

};