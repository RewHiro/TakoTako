#pragma once

#include <memory>

#include "boost\noncopyable.hpp"

class Scene;
enum class SceneType;
using SceneSP = std::unique_ptr < Scene > ;

class SceneFactory : private boost::noncopyable
{
public:

	static SceneFactory& getInstance()
	{
		static SceneFactory instance;
		return instance;
	}

	SceneSP create(const SceneType type)const;
};