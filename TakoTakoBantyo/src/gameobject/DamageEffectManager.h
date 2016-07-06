#pragma once

#include "GameObject.h"

class DamageEffectManager : public GameObject
{
public:

	DamageEffectManager(const ci::Vec3f& pos, const ci::gl::Material& material);
	static std::shared_ptr<DamageEffectManager>const create(const ci::Vec3f& pos, const ci::gl::Material& material);

private:

	const int EFFECT_NUM;
};