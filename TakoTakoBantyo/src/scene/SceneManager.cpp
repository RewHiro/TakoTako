#include "SceneManager.h"

#include "cinder/ImageIo.h"
#include "cinder/app/App.h"
#include "cinder/Rand.h"

#include "ResourceManager.h"

#include "SceneFactory.h"

#include "gameobject/GameObjectTask.h"
#include "gameobject/GameObject.h"


using namespace ci;
using namespace ci::app;

SceneManager::SceneManager()
{
	GameObjectTask::getInstnace();

	Rand::randomize();

	auto& resource_manager = ResourceManager::getInstance();

	resource_manager.add(FontType::MEIRYO, Font(loadAsset("meiryo.ttc"), 24));
	resource_manager.add(FontType::MEIRYO_BOLD, Font(loadAsset("meiryob.ttc"), 48));

	resource_manager.add(TextureType::BACKGROUND, gl::Texture(loadImage(loadAsset("sea.png"))));
	resource_manager.add(TextureType::GROUND, gl::Texture(loadImage(loadAsset("ground.png"))));
	
	resource_manager.add(TextureType::PLAYER01, gl::Texture(loadImage(loadAsset("octopus.png"))));
	resource_manager.add(TextureType::PLAYER02, gl::Texture(loadImage(loadAsset("octopus_3a.png"))));
	resource_manager.add(TextureType::PLAYER03, gl::Texture(loadImage(loadAsset("octopus_4a.png"))));
	resource_manager.add(TextureType::PLAYER04, gl::Texture(loadImage(loadAsset("octopus_2a.png"))));

	resource_manager.add(TextureType::UI_HIT_POINT, gl::Texture(loadImage(loadAsset("ui_hit_point.png"))));
	resource_manager.add(TextureType::PLAYER_FOOT, gl::Texture(loadImage(loadAsset("octopus_arm.png"))));
	resource_manager.add(TextureType::SEAWEED, gl::Texture(loadImage(loadAsset("grass.png"))));
	resource_manager.add(TextureType::ROCK01, gl::Texture(loadImage(loadAsset("rock_1.png"))));
	resource_manager.add(TextureType::ROCK02, gl::Texture(loadImage(loadAsset("rock_2.png"))));
	resource_manager.add(TextureType::ITEM01_BODY, gl::Texture(loadImage(loadAsset("crab_body_tex.png"))));
	resource_manager.add(TextureType::ITEM01_ARM, gl::Texture(loadImage(loadAsset("crab_arm.png"))));
	resource_manager.add(TextureType::METARIC, gl::Texture(loadImage(loadAsset("sphere_map2.png"))));
	resource_manager.add(TextureType::SUN, gl::Texture(loadImage(loadAsset("sun.png"))));
	resource_manager.add(TextureType::CORONA, gl::Texture(loadImage(loadAsset("corona.png"))));
	resource_manager.add(TextureType::MANUAL, gl::Texture(loadImage(loadAsset("manual.png"))));

	resource_manager.add(TextureType::MANUAL01, gl::Texture(loadImage(loadAsset("manual01.png"))));
	resource_manager.add(TextureType::MANUAL02, gl::Texture(loadImage(loadAsset("manual02.png"))));
	resource_manager.add(TextureType::MANUAL03, gl::Texture(loadImage(loadAsset("manual03.png"))));
	resource_manager.add(TextureType::MANUAL04, gl::Texture(loadImage(loadAsset("manual04.png"))));
	resource_manager.add(TextureType::MANUAL05, gl::Texture(loadImage(loadAsset("manual05.png"))));

	resource_manager.add(TextureType::PLAYER02_FOOT, gl::Texture(loadImage(loadAsset("octopus_arm_3.png"))));
	resource_manager.add(TextureType::PLAYER03_FOOT, gl::Texture(loadImage(loadAsset("octopus_arm_4.png"))));
	resource_manager.add(TextureType::PLAYER04_FOOT, gl::Texture(loadImage(loadAsset("octopus_arm_2.png"))));
	resource_manager.add(TextureType::BUBBLE, gl::Texture(loadImage(loadAsset("bubble.png"))));

	resource_manager.add(ModelType::PLAYER_BODY, ObjLoader(loadAsset("octopus_body.obj")));
	resource_manager.add(ModelType::PLAYER_FOOT, ObjLoader(loadAsset("octopus_arm.obj")));
	resource_manager.add(ModelType::ROCK01, ObjLoader(loadAsset("rock_1.obj")));
	resource_manager.add(ModelType::ROCK02, ObjLoader(loadAsset("rock_2.obj")));
	resource_manager.add(ModelType::PLAYER_BULLET, ObjLoader(loadAsset("bullet.obj")));
	resource_manager.add(ModelType::ITEM01_BODY, ObjLoader(loadAsset("crab_body.obj")));
	resource_manager.add(ModelType::ITEM01_ARM, ObjLoader(loadAsset("crab_arm.obj")));

	scene_ = SceneFactory::getInstance().create(SceneType::TITLE);
}

void SceneManager::changeScene(const SceneType type)
{
	scene_.reset();
	scene_ = SceneFactory::getInstance().create(type);
}

void SceneManager::update()
{
	scene_->update(*this);
}

void SceneManager::draw()
{
	scene_->draw();
}

void SceneManager::resize()
{
	scene_->resize();
}