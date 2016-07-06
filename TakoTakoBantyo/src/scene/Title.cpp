#include "Title.h"

#include <stdexcept>

#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "cinder/Unicode.h"

#include "cinder/audio/Context.h"
#include "cinder/audio/NodeEffects.h"
#include "cinder/audio/SamplePlayerNode.h"

#include "input/UtilityGamePad.h"

#include "ResourceManager.h"

#include "SceneManager.h"

#include "gameobject/GameObjectTask.h"
#include "gameobject/Camera.h"
#include "gameobject/BackGround.h"
#include "gameobject/RockManager.h"
#include "gameobject/GroundManager.h"
#include "gameobject/SeaWeedManager.h"
#include "gameobject/ItemManager.h"
#include "gameobject/Bubble.h"

#include "component/Transform.h"


using namespace ci;
using namespace ci::app;
using namespace input;

const float conMaxAxisValue = 1.0f;

Title::Title() :
light_(std::make_shared<gl::Light>(gl::Light::DIRECTIONAL, 0)),
camera_(object::Camera::create(0)->getObject<object::Camera>()),
font_(ResourceManager::getInstance().find(FontType::MEIRYO_BOLD)),
title_font_(loadAsset("meiryob.ttc"), 80),
rotate_(Quatf::identity()),
select_num_(0),
player_num_(0),
bubble_create_count_(0),
is_enter_(false),
is_serect_(false)
{

	for (int i = 0; i < 4; ++i)
	{
		axis_interval_[i] = 0;
	}

	auto& camera_persp = camera_->getCameraPersp();

	camera_persp.setPerspective(90, getWindowAspectRatio(), 0.5f, 1000.0f);
	camera_persp.lookAt(Vec3f(0, 30, -10), Vec3f(0, 20, 10), Vec3f::yAxis());
	camera_->transform()->setPos(Vec3f(0, 5, 0));

	auto ctx = audio::Context::master();


	// connect and enable the Context
	auto mGain = ctx->makeNode(new audio::GainNode(1.0f));

	{

		auto source_file = audio::load(loadAsset("title.mp3"),ctx->getSampleRate());
		mPhase = 0.0f;

		audio::BufferRef buffer = source_file->loadBuffer();
		bgm_ = ctx->makeNode(new audio::BufferPlayerNode(buffer));



		bgm_ >> mGain >> ctx->getOutput();
		bgm_->setLoopEnabled(true);

	}

	{
		auto source_file = audio::load(loadAsset("title_enter.wav"), ctx->getSampleRate());

		audio::BufferRef buffer = source_file->loadBuffer();
		enter_se_ = ctx->makeNode(new audio::BufferPlayerNode(buffer));

		enter_se_ >> mGain;
	}

	ctx->enable();
	bgm_->start();

	light_->setAmbient(ColorA(0.2f, 0.2f, 0.2f));
	light_->setDiffuse(ColorA(0.8f, 0.8f, 0.8f));
	light_->setSpecular(ColorA::white());
	light_->setDirection(Vec3f(1, 1, -1));


	Background::create();
	GroundManager::create();
	SeaWeedManager::create(true);
	RockManager::create();
	ItemManager::create();

	auto view_port = Area(0, 0, getWindowWidth(), getWindowHeight());

	camera_->setViewPortArea(view_port);

	gl::setViewport(view_port);
}

void Title::update(SceneManager& scene_manager)
{

	const auto& gamepad = GamePad::getInstance();

	player_num_ = std::abs(select_num_) % 3 + 2;
	for (int i = 0; i < 4; ++i)
	{
		if (is_enter_)break;
		if (conMaxAxisValue == gamepad.getPressAxis(i, GamePad::AxisID::LEFT_Y))
		{

			if (axis_interval_[i] % 20 == 0)
			{
				++select_num_;
			}
			++axis_interval_[i];
		}
		else if (-conMaxAxisValue == gamepad.getPressAxis(i, GamePad::AxisID::LEFT_Y))
		{
			if (axis_interval_[i] % 20 == 0)
			{
				--select_num_;
			}
			++axis_interval_[i];
		}
		else
		{
			axis_interval_[i] = 0;
		}


		if (gamepad.isPushButton(i, GamePad::ButtonID::START))
		{
			if (!is_enter_)
			{
				enter_se_->start();
				is_enter_ = true;
			}
		}

		if (gamepad.isPushButton(i, GamePad::ButtonID::SELECT))
		{
			is_serect_ = !is_serect_;
		}
	}

	GameObjectTask::getInstnace().update();
	rotate_ *= Quatf(Vec3f::yAxis(), 0.001f);

	auto direction = rotate_ * Vec3f::zAxis();
	direction *= 30;

	auto camera_pos = Vec3f(-direction.x, 30, -direction.z);
	camera_->getCameraPersp().setEyePoint(camera_pos);

	direction.y = 22;
	camera_->getCameraPersp().setCenterOfInterestPoint(direction);

	if (is_enter_)
	{

		if (bubble_create_count_ % 2 == 0)
		{
			Bubble::create(Vec3f::zero(), Vec3f::one() * 1.5f, 0.08f, 0.4f, true);
			Bubble::create(Vec3f::zero(), Vec3f::one() * 1.5f, 0.08f, 0.4f, true);
			Bubble::create(Vec3f::zero(), Vec3f::one() * 1.5f, 0.08f, 0.4f, true);
			Bubble::create(Vec3f::zero(), Vec3f::one() * 1.5f, 0.08f, 0.4f, true);
		}
		bubble_create_count_++;
		if (enter_se_->isEof())
		{
			playerNum() = static_cast<PlayerNum>(player_num_);
			scene_manager.changeScene(SceneType::STAGE);
		}
	}
}

void Title::draw()
{

	auto& game_object_task = GameObjectTask::getInstnace();

	gl::enable(GL_NORMALIZE);
	gl::enableDepthRead();
	gl::enableDepthWrite();
	gl::enable(GL_CULL_FACE);
	gl::enable(GL_LIGHTING);

	gl::pushMatrices();

	gl::setMatrices(camera_->getCameraPersp());



	light_->enable();
	game_object_task.drawNotAlpheBlend();
	gl::disableDepthWrite();
	game_object_task.drawAlphaBlend(camera_);
	light_->disable();

	gl::popMatrices();



	gl::pushMatrices();


	gl::disable(GL_LIGHTING);
	gl::disable(GL_CULL_FACE);
	gl::enableAlphaBlending();



	gl::setMatrices(camera_->getCameraOrtho());

	gl::scale(1, -1);

	drawJapaneseString(
		L"タコタコバンチョウ",
		Vec2f(0, -getWindowCenter().y * 0.5f),
		Vec2f::one()*1.05f,
		ColorA(0, 0, 0),
		title_font_);

	drawJapaneseString(
		L"タコタコバンチョウ",
		Vec2f(0, -getWindowCenter().y * 0.5f),
		Vec2f::one(),
		ColorA::white(),
		title_font_);


	const int diff_pos = 2;

	drawJapaneseString(
		L"何人で遊ぶ？",
		Vec2f(0 + diff_pos, getWindowCenter().y * 0.25f + diff_pos),
		Vec2f::one()*0.75f,
		ColorA(0, 0, 0),
		font_);

	drawJapaneseString(
		L"何人で遊ぶ？",
		Vec2f(0, getWindowCenter().y * 0.25f),
		Vec2f::one()*0.75f,
		ColorA::white(),
		font_);

	auto player_num_wstr = std::to_wstring(player_num_);

	drawJapaneseString(
		player_num_wstr + L"人",
		Vec2f(0 + diff_pos, getWindowCenter().y * 0.35f + diff_pos),
		Vec2f::one()*0.75f,
		ColorA(0, 0, 0),
		font_);

	drawJapaneseString(
		player_num_wstr + L"人",
		Vec2f(0, getWindowCenter().y * 0.35f),
		Vec2f::one()*0.75f,
		ColorA::white(),
		font_);

	drawJapaneseString(
		L"スタートボタンではじめる",
		Vec2f(0 + diff_pos, getWindowCenter().y * 0.7f + diff_pos),
		Vec2f::one()*0.75f,
		ColorA(0, 0, 0),
		font_);

	drawJapaneseString(
		L"スタートボタンではじめる",
		Vec2f(0, getWindowCenter().y * 0.7f),
		Vec2f::one()*0.75f,
		ColorA::white(),
		font_);

	drawJapaneseString(
		L"BACKボタンで遊び方",
		Vec2f(-getWindowCenter().x * 0.8f + diff_pos, -getWindowCenter().y + diff_pos),
		Vec2f::one()*0.5f,
		ColorA(0, 0, 0),
		font_);

	drawJapaneseString(
		L"BACKボタンで遊び方",
		Vec2f(-getWindowCenter().x * 0.8f, -getWindowCenter().y),
		Vec2f::one()*0.5f,
		ColorA::white(),
		font_);

	drawJapaneseString(
		L"Xbox360のコントローラを差してね",
		Vec2f(getWindowCenter().x * 0.7f + diff_pos, -getWindowCenter().y + diff_pos),
		Vec2f::one()*0.5f,
		ColorA(0, 0, 0),
		font_);

	drawJapaneseString(
		L"Xbox360のコントローラを差してね",
		Vec2f(getWindowCenter().x * 0.7f, -getWindowCenter().y),
		Vec2f::one()*0.5f,
		ColorA::white(),
		font_);

	if (is_serect_)
	{
		gl::translate(-getWindowCenter() + getWindowCenter() * 0.38f);
		gl::draw(ResourceManager::getInstance().find(TextureType::MANUAL));
	}

	gl::popMatrices();

	gl::disableDepthRead();
	gl::disableAlphaBlending();
	gl::disable(GL_NORMALIZE);
}

void Title::resize()
{
	gl::setViewport(Area(0, 0, getWindowWidth(), getWindowHeight()));
	camera_->setViewPortArea(Area(0, 0, getWindowWidth(), getWindowHeight()));
}

void Title::drawJapaneseString(const std::wstring& wstring, const ci::Vec2f& pos, const ci::Vec2f& scale, const ci::ColorA color, const ci::Font& font)
{
	gl::pushModelView();

	gl::translate(pos);
	gl::scale(scale);

	gl::drawStringCentered(
		ci::toUtf8((char16_t*)wstring.c_str()),
		Vec2f::zero(),
		color,
		font);

	gl::popModelView();
}