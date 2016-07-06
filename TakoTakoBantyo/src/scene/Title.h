#pragma once

#include "cinder/Font.h"
#include "cinder/audio/Voice.h"
#include "cinder/gl/Light.h"

#include "cinder/audio/Context.h"
#include "cinder/audio/SamplePlayerNode.h"

#include "Scene.h"

namespace object
{
	class Camera;
}

class Title : public Scene
{
public:

	Title();
	~Title(){ bgm_->stop(); }

private:

	void update(SceneManager& scene_manager)final;
	void draw()final;
	void resize()final;

	void drawJapaneseString(const std::wstring& wstring, const ci::Vec2f& pos, const ci::Vec2f& scale = ci::Vec2f::one(), const ci::ColorA color = ci::ColorA::white(), const ci::Font& font = ci::Font());

	std::shared_ptr<object::Camera> camera_;
	std::shared_ptr<ci::gl::Light>light_;
	std::shared_ptr<ci::audio::BufferPlayerNode> bgm_;
	std::shared_ptr<ci::audio::BufferPlayerNode> enter_se_;
	ci::Font font_;
	ci::Font title_font_;
	ci::Quatf rotate_;
	float mPhase;
	int select_num_;
	int axis_interval_[4];
	int player_num_;
	int bubble_create_count_;
	bool is_enter_;
	bool is_serect_;
};