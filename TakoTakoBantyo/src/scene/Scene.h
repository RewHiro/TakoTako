#pragma once

#include <memory>

#include "boost/noncopyable.hpp"

class SceneManager;

enum class SceneType
{
	TITLE,
	STAGE,
};

enum class PlayerNum
{
	ONE = 1,
	TWO,
	THREE,
	FOUR
};

class Scene : private boost::noncopyable
{
public:


	Scene();
	virtual ~Scene() = 0;

	static PlayerNum& playerNum()
	{
		static PlayerNum instance =
			PlayerNum::TWO;
		return instance;
	}

protected:

	virtual void update(SceneManager& scene_manager){}
	virtual void draw(){}
	virtual void resize(){}

private:

	friend class SceneManager;
};

using SceneSP = std::unique_ptr < Scene > ;