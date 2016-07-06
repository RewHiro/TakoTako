#include "GameObjectTask.h"

#include "GameObject.h"

#include "scene/Scene.h"

#include "component/Transform.h"

using namespace ci;

void GameObjectTask::add(
	const GameObjectSP& game_object,
	bool use_alpha_blend,
	bool is_ui,
	int player_num)
{
	if (is_ui)
	{
		switch (player_num)
		{
			case 1:
				game_object_ui_list_[0].emplace_back(game_object);
				break;
			case 2:
				game_object_ui_list_[1].emplace_back(game_object);
				break;
			case 3:
				game_object_ui_list_[2].emplace_back(game_object);
				break;
			case 4:
				game_object_ui_list_[3].emplace_back(game_object);
				break;
		}

	}
	else if (use_alpha_blend)
	{
		game_object_alpha_blend_list_.emplace_back(game_object);
	}
	else
	{
		game_object_not_alpha_blend_list_.emplace_back(game_object);
	}
	game_object_map_.emplace(game_object->getName(), game_object);
}

void GameObjectTask::addCamera(const GameObjectSP& game_object)
{
	game_object_map_.emplace(game_object->getName(), game_object);
}

GameObjectSP GameObjectTask::find(const std::string& name)const
{
	if (game_object_map_.find(name) == game_object_map_.end())
	{
		return nullptr;
	}
	return game_object_map_.find(name)->second;
}

std::pair<GAMEOBJ_MAP_ITR, GAMEOBJ_MAP_ITR>& GameObjectTask::finds(const std::string& name)
{
	return game_object_map_.equal_range(name);
}

void GameObjectTask::update()
{
	for (auto& object : game_object_not_alpha_blend_list_)
	{
		if (object->isUpdate())
		{
			object->update();
			object->transform()->destoryer();
			object->component_task_.update();
		}
		else
		{
			object->start();
			object->changeUpdate();
			object->component_task_.update();
		}
	}

	for (auto& object : game_object_alpha_blend_list_)
	{
		if (object->isUpdate())
		{
			object->update();
			object->transform()->destoryer();
			object->component_task_.update();
		}
		else
		{
			object->start();
			object->changeUpdate();
			object->component_task_.update();
		}
	}

	for (auto& object_list : game_object_ui_list_)
	{
		for (auto& object : object_list){
			if (object->isUpdate())
			{
				object->update();
				object->transform()->destoryer();
				object->component_task_.update();
			}
			else
			{
				object->start();
				object->changeUpdate();
				object->component_task_.update();
			}
		}
	}
	destory();
}

void GameObjectTask::drawNotAlpheBlend()
{
	for (auto& object : game_object_not_alpha_blend_list_)
	{
		if (object->isStart())continue;
		object->draw();
	}
}

void GameObjectTask::drawAlphaBlend(const std::weak_ptr<object::Camera>& camera)
{
	gl::enableAlphaBlending();

	for (auto& object : game_object_alpha_blend_list_)
	{
		gl::pushModelView();
		gl::multModelView(object->transform()->worldMatrix());
		object->transform()->setProjectionMatrix(gl::getModelView());
		gl::popModelView();
	}

	game_object_alpha_blend_list_.sort(
		[](std::weak_ptr<GameObject> lth, std::weak_ptr<GameObject> rth)
	{
		return lth.lock()->transform()->getProjectionPos().z < rth.lock()->transform()->getProjectionPos().z;
	}
	);

	for (auto& object : game_object_alpha_blend_list_)
	{
		if (object->isStart())continue;
		object->draw(camera);
	}
	gl::disableAlphaBlending();
}

void GameObjectTask::drawUI(int player_num)
{
	for (auto& object : game_object_ui_list_[player_num - 1])
	{
		if (object->isStart())continue;
		object->draw();
	}
}

void GameObjectTask::clear()
{
	game_object_map_.clear();
	game_object_not_alpha_blend_list_.clear();
	game_object_alpha_blend_list_.clear();
	game_object_ui_list_.clear();
}

void GameObjectTask::resize()
{
	for (auto& object_list : game_object_ui_list_)
	{
		for (auto& object : object_list)
		{
			object->resize();
		}
	}
}

void GameObjectTask::setup()
{
	auto player_num = static_cast<int>(Scene::playerNum());
	for (auto i = 0; i < player_num; ++i)
	{
		game_object_ui_list_.emplace_back();
	}
}

void GameObjectTask::destory()
{
	auto itr = game_object_map_.cbegin();
	while (itr != game_object_map_.cend())
	{

		if (itr->second->isDelete())
		{
			itr = game_object_map_.erase(itr);
		}
		else
		{
			++itr;
		}
	}

	game_object_not_alpha_blend_list_.remove_if([](const GameObjectSP& game_object){return game_object->isDelete(); });
	game_object_alpha_blend_list_.remove_if([](const GameObjectSP& game_object){return game_object->isDelete(); });
	for (auto& object_array : game_object_ui_list_)
	{
		object_array.remove_if([](const GameObjectSP& game_object){return game_object->isDelete(); });
	}
}