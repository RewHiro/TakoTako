#include "DamageEffectManager.h"

#include "cinder/Rand.h"

#include "ResourceManager.h"

#include "DamageEffect.h"
#include "GameObjectTask.h"

#include "component/Transform.h"

using namespace ci;

DamageEffectManager::DamageEffectManager(const ci::Vec3f& pos, const ci::gl::Material& material) :
GameObject("DamageEffectManager"),
EFFECT_NUM(8)
{
	auto texture = ResourceManager::getInstance().find(TextureType::BUBBLE);
	for (auto i = 0; i < EFFECT_NUM; ++i)
	{
		auto direction = Rand::randVec3f();
		direction.y = 1.0f;
		auto damage_effect = DamageEffect::create(pos, Vec3f::one(), direction, material, texture, 1.0f, 0.0f, Rand::randFloat(0.1f, 0.2f));
		damage_effect->transform()->setParent(transform_);
	}
}

std::shared_ptr<DamageEffectManager>const DamageEffectManager::create(const ci::Vec3f& pos, const ci::gl::Material& material)
{
	auto instance = std::make_shared<DamageEffectManager>(pos, material);
	GameObjectTask::getInstnace().add(instance);
	return instance;
}