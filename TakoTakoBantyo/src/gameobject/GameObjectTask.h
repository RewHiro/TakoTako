#pragma once

#include <unordered_map>
#include <list>
#include <memory>

#include "boost/noncopyable.hpp"

#include "Using.h"


namespace object
{
	class Camera;
}

class GameObjectTask : private boost::noncopyable
{
public:

	static GameObjectTask& getInstnace()
	{
		static GameObjectTask instance;
		return instance;
	}

	void add(
		const GameObjectSP& game_object,
		bool use_alpha_blend = false,
		bool is_ui = false,
		int player_num = 1);

	void addCamera(const GameObjectSP& game_object);

	GameObjectSP find(const std::string& name)const;
	std::pair<GAMEOBJ_MAP_ITR, GAMEOBJ_MAP_ITR>& finds(const std::string& name);

	void update();
	void drawNotAlpheBlend();
	void drawAlphaBlend(const std::weak_ptr<object::Camera>& camera);
	void drawUI(int player_num = 1);
	void clear();
	void resize();
	void setup();

private:

	void destory();

	std::unordered_multimap<std::string, GameObjectSP>game_object_map_;
	std::list<GameObjectSP>game_object_not_alpha_blend_list_;
	std::list<GameObjectSP>game_object_alpha_blend_list_;
	std::vector<std::list<GameObjectSP>>game_object_ui_list_;
};