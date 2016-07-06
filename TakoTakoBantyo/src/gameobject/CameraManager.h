#pragma once

#include "GameObject.h"

class CameraManager : public GameObject
{
public:

	CameraManager(const ci::Quatf& rotate);
	static std::shared_ptr<CameraManager>const create(const ci::Quatf& rotate);
};