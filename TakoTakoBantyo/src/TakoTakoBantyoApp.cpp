#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "input/UtilityGamePad.h"

#include "scene/SceneManager.h"

#ifdef DEBUG
#pragma comment(lib,"Gamepad_d.lib")
#pragma comment(lib,"Winmm.lib")

#pragma comment(lib,"Bullet2FileLoader_Debug.lib")
#pragma comment(lib,"Bullet3Collision_Debug.lib")
#pragma comment(lib,"Bullet3Common_Debug.lib")
#pragma comment(lib,"Bullet3Dynamics_Debug.lib")
#pragma comment(lib,"Bullet3Geometry_Debug.lib")
#pragma comment(lib,"Bullet3OpenCL_clew_Debug.lib")
#pragma comment(lib,"BulletCollision_Debug.lib")
#pragma comment(lib,"BulletDynamics_Debug.lib")
#pragma comment(lib,"BulletFileLoader_Debug.lib")
#pragma comment(lib,"BulletSoftBody_Debug.lib")
#pragma comment(lib,"BulletWorldImporter_Debug.lib")
#pragma comment(lib,"BulletXmlWorldImporter_Debug.lib")
#pragma comment(lib,"ConvexDecomposition_Debug.lib")
#pragma comment(lib,"GIMPACTUtils_Debug.lib")
#pragma comment(lib,"gtest_Debug.lib")
#pragma comment(lib,"gwen_Debug.lib")
#pragma comment(lib,"HACD_Debug.lib")
#pragma comment(lib,"LinearMath_Debug.lib")
#pragma comment(lib,"OpenGLWindow_Debug.lib")

#else
#pragma comment(lib,"Gamepad.lib")
#pragma comment(lib,"Winmm.lib")

#pragma comment(lib,"Bullet2FileLoader.lib")
#pragma comment(lib,"Bullet3Collision.lib")
#pragma comment(lib,"Bullet3Common.lib")
#pragma comment(lib,"Bullet3Dynamics.lib")
#pragma comment(lib,"Bullet3Geometry.lib")
#pragma comment(lib,"Bullet3OpenCL_clew.lib")
#pragma comment(lib,"BulletCollision.lib")
#pragma comment(lib,"BulletDynamics.lib")
#pragma comment(lib,"BulletFileLoader.lib")
#pragma comment(lib,"BulletSoftBody.lib")
#pragma comment(lib,"BulletWorldImporter.lib")
#pragma comment(lib,"BulletXmlWorldImporter.lib")
#pragma comment(lib,"ConvexDecomposition.lib")
#pragma comment(lib,"GIMPACTUtils.lib")
#pragma comment(lib,"gtest.lib")
#pragma comment(lib,"gwen.lib")
#pragma comment(lib,"HACD.lib")
#pragma comment(lib,"LinearMath.lib")
#pragma comment(lib,"OpenGLWindow.lib")

#endif


using namespace ci;
using namespace ci::app;
using namespace input;

class TakoTakoBantyoApp : public AppNative
{

private:

	void prepareSettings(Settings *settings)final
	{
		settings->setFullScreen();
		settings->setWindowSize(1440, 860);
		settings->disableFrameRate();
		//settings->setFrameRate(60);
	}

	void setup()final
	{
		GamePad::getInstance().init();
		scene_manager_ = std::make_unique<SceneManager>();
	}

	void shutdown()final
	{
		GamePad::getInstance().shutdown();
	}
	
	void update()final
	{
		GamePad::getInstance().update();
		scene_manager_->update();
	}
	
	void draw()final
	{
		gl::clear();
		scene_manager_->draw();
		GamePad::getInstance().flush();
	}

	void resize()final
	{
		scene_manager_->resize();
	}


	//　cinderの設定が終わってから生成したいのでポインタにしている
	std::unique_ptr<SceneManager>scene_manager_;
};

CINDER_APP_NATIVE(TakoTakoBantyoApp, RendererGl)
