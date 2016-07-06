#pragma once

#include "boost/noncopyable.hpp"

// Memo:unique_ptr�͑O���錾�����Ő����ł��Ȃ�
#include "Scene.h"

class TakoTakoBantyoApp;

class SceneManager : private boost::noncopyable
{
public:
	
	SceneManager();

	void changeScene(const SceneType type);

private:

	void update();
	void draw();
	void resize();



	friend class TakoTakoBantyoApp;

	SceneSP scene_;
};