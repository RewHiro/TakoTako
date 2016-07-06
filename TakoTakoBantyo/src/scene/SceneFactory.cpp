#include "SceneFactory.h"

#include <unordered_map>

#include "Title.h"
#include "Stage.h"

SceneSP SceneFactory::create(const SceneType type)const
{
	static std::unordered_map<SceneType, std::function<SceneSP()>> scene_list =
	{
		{ SceneType::TITLE, []{return std::make_unique<Title>(); } },
		{ SceneType::STAGE, []{return std::make_unique<Stage>(); } },
	};

	return std::move((scene_list.find(type)->second)());
}