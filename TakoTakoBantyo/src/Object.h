#pragma once

#include <string>

#include "boost/noncopyable.hpp"

#include "cinder/TriMesh.h"
#include "cinder/gl/Material.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "cinder/audio/Voice.h"

class GameObject;

class Object : private boost::noncopyable
{
public:

	explicit Object(const std::string& name = "empty");
	virtual ~Object() = 0{}

	virtual void onCollisionEnter(GameObject& gameobject){}
	virtual void onCollisionExit(GameObject& gameobject){}
	virtual void momentCollisionExit(GameObject& gameobject){}

	bool isDelete()const{ return is_delete_; }
	std::string getName()const{ return name_; }
	size_t getHashCode()const{ return std::hash<std::string>()(name_); }
	
	virtual void destroy(){ is_delete_ = true; }
protected:

	enum class UpdateState
	{
		START,
		UPDATE
	};

	virtual void start(){}
	virtual void update(){}

	bool isStart()const { return update_state_ == UpdateState::START; }
	bool isUpdate()const{ return update_state_ == UpdateState::UPDATE; }

	void changeUpdate(){ update_state_ = UpdateState::UPDATE; }

	UpdateState update_state_;
	std::string name_;
	bool is_delete_;


private:
};