#include "UtilityGamePad.h"
#include "cinder/app/App.h"

#include "scene/Scene.h"

using namespace ci;
using namespace ci::app;



namespace input
{
	extern "C" {
		#include "Gamepad.h"
	}
	bool isRemove = false;

	void GamePad::onButtonDown(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context)
	{
		auto& gamepad = GamePad::getInstance();
		auto deviceID = device->deviceID % static_cast<int>(Scene::playerNum());
		gamepad.setDevice(deviceID,device->vendorID);
		gamepad.setButtonDown(deviceID, buttonID);
		console() << "Button " << buttonID << " down on device " << device->deviceID << " at " << timestamp << " with context " << context << std::endl;
	}

	void GamePad::onButtonUp(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context)
	{
		auto& gamepad = GamePad::getInstance();
		auto deviceID = device->deviceID % static_cast<int>(Scene::playerNum());
		gamepad.setDevice(deviceID, device->vendorID);
		gamepad.setButtonUp(deviceID, buttonID);
		console() << "Button " << buttonID << " up on device " << device->deviceID << " at " << timestamp << " with context " << context << std::endl;
	}

	void GamePad::onAxisMoved(struct Gamepad_device * device, unsigned int axisID, float value, float lastValue, double timestamp, void * context)
	{
		auto& gamepad = GamePad::getInstance();
		auto deviceID = device->deviceID % static_cast<int>(Scene::playerNum());
		gamepad.setDevice(deviceID, device->vendorID);
		gamepad.erasePressAxis(deviceID,axisID);
		if(value > 0.4f || value < -0.4f)
		{
			gamepad.setAxis(deviceID, axisID, value);
			console() << "Axis " << axisID << " moved from " << lastValue << " to " << value << " on device " << device->deviceID << " at " << timestamp << " with context " << context << std::endl;
		}
	}

	void GamePad::onDeviceAttached(struct Gamepad_device * device, void * context)
	{
		//if(isRemove)
		//{
		//	auto a = device->deviceID;
		//	device->deviceID = a - 1;
		//	isRemove = false;
		//}
		console() << "Device ID " << device->deviceID << " attached (vendor = " << device->vendorID << "; product = " << device->productID << ") with context" << context << std::endl;
	}

	void GamePad::onDeviceRemoved(struct Gamepad_device * device, void * context)
	{
		console() << "Device ID " << device->deviceID << " removed with context " << context << std::endl;
		isRemove = true;
	}

	void GamePad::erasePressAxis(int device_id,int axis_id)
	{
		const auto& find = press_axis.find(std::make_tuple(device_id, axis_id));
		if (find == press_axis.end())return;
		press_axis.erase(find);
	}

	void GamePad::setAxis(int device_id, int axis_id, float value)
	{
		press_axis.emplace(std::make_tuple(device_id, axis_id), value);
	}

	void GamePad::setButtonDown(int device_id, int button_id)
	{
		push_button.emplace(std::make_tuple(device_id, button_id));
		press_button.emplace(std::make_tuple(device_id, button_id));
	}

	void GamePad::setButtonUp(int device_id, int button_id)
	{
		pull_button.emplace(std::make_tuple(device_id, button_id));
		const auto& find = press_button.find(std::make_tuple(device_id, button_id));
		press_button.erase(find);
	}

	void GamePad::setup()
	{
		Gamepad_deviceAttachFunc(&GamePad::onDeviceAttached, (void *)0x1);
		Gamepad_deviceRemoveFunc(&GamePad::onDeviceRemoved, (void *)0x2);
		Gamepad_buttonDownFunc(&GamePad::onButtonDown, (void *)0x3);
		Gamepad_buttonUpFunc(&GamePad::onButtonUp, (void *)0x4);
		Gamepad_axisMoveFunc(&GamePad::onAxisMoved, (void *)0x5);
	}
	void GamePad::init()
	{
		Gamepad_init();
		setup();
	}

	void GamePad::shutdown()
	{
		Gamepad_shutdown();
	}

#define POLL_ITERATION_INTERVAL 30
	void GamePad::update()
	{
		static unsigned int iterationsToNextPoll = POLL_ITERATION_INTERVAL;

		iterationsToNextPoll--;
		if (iterationsToNextPoll == 0) {
			Gamepad_detectDevices();
			iterationsToNextPoll = POLL_ITERATION_INTERVAL;
		}
		Gamepad_processEvents();
	}

	void GamePad::flush()
	{
		push_button.clear();
		pull_button.clear();
	}

	void GamePad::setDevice(int device_id, int vendor_id)
	{
		vendor_map.emplace(device_id, vendor_id);
	}

	void GamePad::clear()
	{
		press_axis.clear();
		push_axis.clear();
		pull_axis.clear();
		push_button.clear();
		press_button.clear();
		pull_button.clear();
	}

	GamePad& GamePad::getInstance()
	{
		static GamePad instance;
		return instance;
	}
};