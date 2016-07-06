#pragma once

#include "Scene.h"
#include <vector>

#include "cinder/gl/Light.h"
#include "cinder/audio/Voice.h"

#include "gameobject/Camera.h"

class Stage : public Scene
{
public:

	Stage();
	~Stage()
	{
		for (auto& bgm : bgms_)
		{
			bgm->stop();
		}
	}

	static int& getResultCount()
	{ 
		static int result_count_ = 1; 
		return result_count_;
	}

private:

	void update(SceneManager& scene_manager)final;
	void draw()final;
	void resize()final;

	void setViewPort();
	void manualUpdate();
	void manualDraw();
	void resultDraw();

	std::vector<std::shared_ptr<object::Camera>>cameras_;
	std::unique_ptr<ci::gl::Light>light_;

	std::vector<std::shared_ptr<cinder::audio::BufferPlayerNode>> bgms_;

	ci::gl::Texture manual_texture_;

	int player_win_num_;
	int manual_page_;
	int axis_interval_[4];
	bool is_result_;
	bool bgm_switch_;
	bool is_manual_;
};