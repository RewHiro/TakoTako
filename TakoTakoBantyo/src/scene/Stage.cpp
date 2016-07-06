#include "Stage.h"

#include "cinder/app/App.h"
#include "cinder/Unicode.h"
#include "cinder/Rand.h"

#include "cinder/audio/Context.h"
#include "cinder/audio/NodeEffects.h"
#include "cinder/audio/SamplePlayerNode.h"

#include "SceneManager.h"

#include "ResourceManager.h"

#include "input/UtilityGamePad.h"

#include "gameobject/GameObject.h"
#include "gameobject/GameObjectTask.h"
#include "gameobject/Player.h"
#include "gameobject/CameraManager.h"
#include "gameobject/Background.h"
#include "gameobject/GroundManager.h"
#include "gameobject/UIManager.h"
#include "gameobject/SeaWeedManager.h"
#include "gameobject/RockManager.h"
#include "gameobject/ItemManager.h"

#include "component/Transform.h"

using  namespace ci;
using  namespace ci::app;
using namespace input;

Stage::Stage() :
light_(std::make_unique<ci::gl::Light>(gl::Light::DIRECTIONAL, 0)),
manual_texture_(ResourceManager::getInstance().find(TextureType::MANUAL01)),
player_win_num_(0),
manual_page_(0),
is_result_(false),
bgm_switch_(false),
is_manual_(true)
{
	for (int i = 0; i < 4; ++i)
	{
		axis_interval_[i] = 0;
	}


	auto ctx = audio::Context::master();
	auto mGain = ctx->makeNode(new audio::GainNode(1.0f));

	{
		auto source_file = audio::load(loadAsset("stage02.mp3"), ctx->getSampleRate());

		audio::BufferRef buffer = source_file->loadBuffer();
		auto bgm = ctx->makeNode(new audio::BufferPlayerNode(buffer));

		bgm >> mGain >> ctx->getOutput();

		ctx->enable();

		bgms_.emplace_back(bgm);
		bgms_[0]->start();
	}

	{
		auto source_file = audio::load(loadAsset("stage03.mp3"), ctx->getSampleRate());
		audio::BufferRef buffer = source_file->loadBuffer();
		auto bgm = ctx->makeNode(new audio::BufferPlayerNode(buffer));

		bgm >> mGain >> ctx->getOutput();

		bgms_.emplace_back(bgm);
	}

	getResultCount() = 1;

	GameObjectTask::getInstnace().setup();
	auto player_num = static_cast<int>(playerNum());

	light_->setAmbient(ColorA(0.2f, 0.2f, 0.2f));
	light_->setDiffuse(ColorA(0.8f, 0.8f, 0.8f));
	light_->setSpecular(ColorA::white());
	light_->setDirection(Vec3f(1, 1, -1));

	const float set_pos = 80.0f;

	for (auto i = 0; i < player_num; ++i)
	{
		cameras_.emplace_back(object::Camera::create(i + 1)->getObject<object::Camera>());

		float y = -0.5f;
		Vec3f pos = Vec3f(0, y, -set_pos);
		Quatf rotate = Quatf::identity();
		PlayerType player_type = PlayerType::ONE;
		switch (i)
		{
			case 1:
				pos = Vec3f(0, y, set_pos);
				rotate = Quatf(Vec3f::yAxis(), M_PI);
				player_type = PlayerType::TWO;
				break;
			case 2:
				pos = Vec3f(-set_pos, y, 0);
				rotate = Quatf(Vec3f::yAxis(), M_PI*0.5f);
				player_type = PlayerType::THREE;
				break;
			case 3:
				pos = Vec3f(set_pos, y, 0);
				rotate = Quatf(Vec3f::yAxis(), M_PI*0.5f + M_PI);
				player_type = PlayerType::FOUR;
				break;
		}
		auto player = Player::create(i + 1, pos, rotate, player_type);
		auto camera_manager = CameraManager::create(rotate);
		camera_manager->transform()->setParent(player->transform());
		cameras_.at(i)->transform()->setParent(camera_manager->transform());
		UIManager::create(player);
	}

	setViewPort();

	Background::create();
	GroundManager::create();
	SeaWeedManager::create();
	RockManager::create();
	ItemManager::create();

	for (auto& camera : cameras_)
	{
		camera->start();
	}

	for (auto& camera : cameras_)
	{
		camera->update();
	}
	GameObjectTask::getInstnace().update();
}

void Stage::update(SceneManager& scene_manager)
{
	manualUpdate();
	for (auto& camera : cameras_)
	{
		camera->update();
	}

	if (!is_manual_)
	{
		GameObjectTask::getInstnace().update();
	}

	auto player_num = static_cast<int>(playerNum());

	if (getResultCount() == player_num)
	{
		for (auto i = 0; i < player_num; ++i)
		{
			auto player = GameObject::findObject("Player" + std::to_string(i + 1))->getObject<Player>();
			if (player->IsResult())continue;
			player->changeWin();
			is_result_ = true;
			player_win_num_ = i;
			break;
		}
	}

	if (bgm_switch_)
	{
		if (bgms_[1]->isEof())
		{
			bgms_[0]->start();
			bgm_switch_ = !bgm_switch_;
		}
	}
	else
	{
		if (bgms_[0]->isEof())
		{
			bgms_[1]->start();
			bgm_switch_ = !bgm_switch_;
		}
	}

	const auto& gamepad = GamePad::getInstance();
	if (is_result_)
	{
		for (auto i = 0; i < player_num; ++i)
		{
			if (gamepad.isPushButton(i, GamePad::ButtonID::START))
			{
				scene_manager.changeScene(SceneType::TITLE);
			}
		}
	}

	for (auto i = 0; i < player_num; ++i)
	{
		if (gamepad.isPressButton(i, GamePad::ButtonID::SELECT) && gamepad.isPressButton(i, GamePad::ButtonID::START))
		{
			scene_manager.changeScene(SceneType::TITLE);
		}
	}
}

void Stage::draw()
{

	auto& game_object_task = GameObjectTask::getInstnace();

	gl::enable(GL_NORMALIZE);
	gl::enableDepthRead();

	gl::pushMatrices();

	auto player_num = 1;
	for (auto& camera : cameras_)
	{
		gl::enable(GL_CULL_FACE);
		gl::setViewport(camera->getViewPortArea());
		gl::setMatrices(camera->getCameraPersp());

		gl::enable(GL_LIGHTING);
		light_->enable();
		gl::enableDepthWrite();
		game_object_task.drawNotAlpheBlend();
		gl::disableDepthWrite();
		game_object_task.drawAlphaBlend(camera);
		light_->disable();
		gl::disable(GL_LIGHTING);


		gl::setMatrices(camera->getCameraOrtho());
		gl::enableAlphaBlending();
		gl::disable(GL_CULL_FACE);

		game_object_task.drawUI(player_num);

		gl::disableAlphaBlending();
		++player_num;
	}
	gl::popMatrices();

	manualDraw();
	resultDraw();

	gl::disableDepthRead();
	gl::disable(GL_NORMALIZE);
}

void Stage::resize()
{
	setViewPort();
	for (auto& camera : cameras_)
	{
		camera->resize();
	}
	GameObjectTask::getInstnace().resize();
}

void Stage::setViewPort()
{
	switch (playerNum())
	{
		case PlayerNum::ONE:
			cameras_.at(0)->setViewPortArea(Area(0, 0, getWindowWidth(), getWindowHeight()));
			break;
		case PlayerNum::TWO:

			cameras_.at(0)->setViewPortArea(Area(0, getWindowHeight() * 0.5f, getWindowWidth(), getWindowHeight() - 1));
			cameras_.at(1)->setViewPortArea(Area(0, 0, getWindowWidth(), getWindowHeight() * 0.5f - 1));

			break;
		case PlayerNum::THREE:

			//　横、高さ、横、高さ
			cameras_.at(0)->setViewPortArea(Area(getWindowWidth() * 0.25f, getWindowHeight() * 0.5f, getWindowWidth() * 0.75f, getWindowHeight() - 1));
			cameras_.at(1)->setViewPortArea(Area(0, 0, getWindowWidth() * 0.5f - 1, getWindowHeight() * 0.5f - 1));
			cameras_.at(2)->setViewPortArea(Area(getWindowWidth() * 0.5f + 1, 0, getWindowWidth(), getWindowHeight()*0.5f - 1));

			break;
		case PlayerNum::FOUR:
			cameras_.at(0)->setViewPortArea(Area(0, getWindowHeight() * 0.5f, getWindowWidth() * 0.5f - 1, getWindowHeight() - 1));
			cameras_.at(1)->setViewPortArea(Area(getWindowWidth() * 0.5f + 1, getWindowHeight() * 0.5f, getWindowWidth(), getWindowHeight() - 1));
			cameras_.at(2)->setViewPortArea(Area(0, 0, getWindowWidth() * 0.5f - 1, getWindowHeight()*0.5f - 1));
			cameras_.at(3)->setViewPortArea(Area(getWindowWidth() * 0.5f + 1, 0, getWindowWidth(), getWindowHeight()*0.5f - 1));
			break;
	}
}

void Stage::manualUpdate()
{
	if (!is_manual_)return;

	auto player_num = static_cast<int>(playerNum());
	const auto& gamepad = GamePad::getInstance();

	for (auto i = 0; i < player_num; ++i)
	{
		if (1.0f == gamepad.getPressAxis(i, GamePad::AxisID::LEFT_X))
		{
			if (axis_interval_[i] % 210 == 0)
			{
				++manual_page_;
			}
			++axis_interval_[i];
		}
		else if (-1.0f == gamepad.getPressAxis(i, GamePad::AxisID::LEFT_X))
		{
			if (axis_interval_[i] % 210 == 0)
			{
				--manual_page_;
				if(manual_page_ <= 0)
				{
					manual_page_ = 0;
				}
			}
			++axis_interval_[i];
		}
		else
		{
			axis_interval_[i] = 0;
		}
	}

	switch (manual_page_)
	{
		case 0:
			manual_texture_ = ResourceManager::getInstance().find(TextureType::MANUAL01);
			break;
		case 1:
			manual_texture_ = ResourceManager::getInstance().find(TextureType::MANUAL02);
			break;
		case 2:
			manual_texture_ = ResourceManager::getInstance().find(TextureType::MANUAL03);
			break;
		case 3:
			manual_texture_ = ResourceManager::getInstance().find(TextureType::MANUAL04);
			break;
		case 4:
			manual_texture_ = ResourceManager::getInstance().find(TextureType::MANUAL05);
			break;
	}



	if(manual_page_ >= 5)
	{
		is_manual_ = false;
	}
}

void Stage::manualDraw()
{
	if (!is_manual_)return;
	gl::pushMatrices();
	gl::enableAlphaBlending();
	gl::setViewport(Area(0, 0, getWindowWidth(), getWindowHeight()));

	gl::color(ColorA(0, 0, 0, 0.5f));
	gl::drawSolidRect(Rectf(0, 0, getWindowWidth(), getWindowHeight()));

	gl::color(ColorA::white());
	gl::translate(getWindowCenter() * 0.3f);
	gl::draw(manual_texture_, Rectf(0, 0, getWindowWidth() * 0.7f, getWindowHeight() * 0.7f));
	gl::popMatrices();


	gl::pushMatrices();
	gl::setViewport(Area(0, 0, getWindowWidth(), getWindowHeight()));
	gl::translate(getWindowCenter());
	gl::translate(0, -400, 0);
	std::wstring str = L"左スティックを右に傾けて次のページに進む";

	gl::drawStringCentered(
		ci::toUtf8((char16_t*)str.c_str()),
		Vec2f::zero(),
		Color::white(),
		ResourceManager::getInstance().find(FontType::MEIRYO_BOLD));

	gl::disableAlphaBlending();
	gl::popMatrices();
}

void Stage::resultDraw()
{
	if (!is_result_)return;
	gl::pushMatrices();
	gl::enableAlphaBlending();
	gl::setViewport(Area(0, 0, getWindowWidth(), getWindowHeight()));
	gl::translate(getWindowCenter());

	std::wstring str = L"スタートボタンでタイトルに戻る";

	gl::drawStringCentered(
		ci::toUtf8((char16_t*)str.c_str()),
		Vec2f::zero(),
		Color::white(),
		ResourceManager::getInstance().find(FontType::MEIRYO_BOLD));

	gl::disableAlphaBlending();
	gl::popMatrices();
}