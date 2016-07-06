#pragma once
#include <map>
#include <set>

class TakoTakoBantyoApp;

namespace input
{
	class GamePad
	{

	private:

		friend class TakoTakoBantyoApp;

		std::map<std::tuple<int, int>, float>press_axis;
		std::map<std::tuple<int, int>, float>push_axis;
		std::map<std::tuple<int, int>, float>pull_axis;

		std::set<std::tuple<int,int>>press_button;
		std::set<std::tuple<int, int>>push_button;
		std::set<std::tuple<int, int>>pull_button;

		std::map<int, int>vendor_map;

		GamePad() = default;

		static void onButtonDown(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context);

		static void onButtonUp(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context);

		static void onAxisMoved(struct Gamepad_device * device, unsigned int axisID, float value, float lastValue, double timestamp, void * context);

		static void onDeviceAttached(struct Gamepad_device * device, void * context);

		static void onDeviceRemoved(struct Gamepad_device * device, void * context);

		void setAxis(int device_id, int axis_id, float value);
		void erasePressAxis(int device_id, int axis_id);

		void setButtonDown(int device_id, int button_id);
		void setButtonUp(int device_id, int button_id);

		void setup();

		void shutdown();
		void update();
		void flush();
		void setDevice(int device_id, int vendor_id);


	public:

		enum class AxisID
		{
			LEFT_X,
			LEFT_Y,
			RIGHT_TRIGGER,
			LEFT_TRIGGER,
			RIGHT_Y,
			RIGHT_X,
			ARROW_X,
			ARROW_Y,
		};

		enum class ButtonID
		{
			DOWN,
			RIGHT,
			LEFT,
			UP,
			LEFT_BUTTON,
			RIGHT_BUTTON,
			SELECT,
			START,
			L2,
			R2,
			L3,
			R3
		};

		int getAxisID(int vender_id,AxisID axis_id)const
		{
			static std::map<std::tuple<int, AxisID>, int> gamepad_map =
			{
				{ std::make_tuple(1133, AxisID::LEFT_X), 0 },
				{ std::make_tuple(1133, AxisID::LEFT_Y), 1 },
				{ std::make_tuple(1133, AxisID::RIGHT_TRIGGER), 2 },
				{ std::make_tuple(1133, AxisID::LEFT_TRIGGER), 2 },
				{ std::make_tuple(1133, AxisID::RIGHT_Y), 3 },
				{ std::make_tuple(1133, AxisID::RIGHT_X), 4 },
				{ std::make_tuple(1133, AxisID::ARROW_X), 5 },
				{ std::make_tuple(1133, AxisID::ARROW_Y), 6 },

				{ std::make_tuple(1356, AxisID::LEFT_X), 0 },
				{ std::make_tuple(1356, AxisID::LEFT_Y), 1 },
				{ std::make_tuple(1356, AxisID::RIGHT_X), 2 },
				{ std::make_tuple(1356, AxisID::RIGHT_Y), 3 },
				{ std::make_tuple(1356, AxisID::RIGHT_TRIGGER), 4 },
				{ std::make_tuple(1356, AxisID::LEFT_TRIGGER), 5 },
				{ std::make_tuple(1356, AxisID::ARROW_X), 6 },
				{ std::make_tuple(1356, AxisID::ARROW_Y), 7 },

				{ std::make_tuple(6708, AxisID::LEFT_X), 0 },
				{ std::make_tuple(6708, AxisID::LEFT_Y), 1 },
				{ std::make_tuple(6708, AxisID::RIGHT_X), 2 },
				{ std::make_tuple(6708, AxisID::RIGHT_Y), 3 },
				{ std::make_tuple(6708, AxisID::ARROW_X), 4 },
				{ std::make_tuple(6708, AxisID::ARROW_Y), 5 },

				{ std::make_tuple(1464, AxisID::LEFT_X), 0 },
				{ std::make_tuple(1464, AxisID::LEFT_Y), 1 },
				{ std::make_tuple(1464, AxisID::RIGHT_X), 2 },
				{ std::make_tuple(1464, AxisID::RIGHT_Y), 3 },
				{ std::make_tuple(1464, AxisID::ARROW_X), 4 },
				{ std::make_tuple(1464, AxisID::ARROW_Y), 5 },

				{ std::make_tuple(7640, AxisID::LEFT_X), 0 },
				{ std::make_tuple(7640, AxisID::LEFT_Y), 1 },
				{ std::make_tuple(7640, AxisID::RIGHT_X), 2 },
				{ std::make_tuple(7640, AxisID::RIGHT_Y), 3 },
				{ std::make_tuple(7640, AxisID::ARROW_X), 4 },
				{ std::make_tuple(7640, AxisID::ARROW_Y), 5 },


				{ std::make_tuple(1118, AxisID::LEFT_X), 0 },
				{ std::make_tuple(1118, AxisID::LEFT_Y), 1 },
				{ std::make_tuple(1118, AxisID::RIGHT_X), 4 },
				{ std::make_tuple(1118, AxisID::RIGHT_Y), 3 },
			};

			return gamepad_map.find(std::make_tuple(vender_id, axis_id))->second;
		}

		void init();

		int getButtonID(int vender_id, ButtonID button_id)const
		{
			static std::map<std::tuple<int, ButtonID>, int> gamepad_map =
			{
				{ std::make_tuple(1133, ButtonID::DOWN), 0 },
				{ std::make_tuple(1133, ButtonID::RIGHT), 1 },
				{ std::make_tuple(1133, ButtonID::LEFT), 2 },
				{ std::make_tuple(1133, ButtonID::UP), 3 },
				{ std::make_tuple(1133, ButtonID::LEFT_BUTTON), 4 },
				{ std::make_tuple(1133, ButtonID::RIGHT_BUTTON), 5 },
				{ std::make_tuple(1133, ButtonID::SELECT), 6 },
				{ std::make_tuple(1133, ButtonID::START), 7 },
				{ std::make_tuple(1133, ButtonID::L3), 8 },
				{ std::make_tuple(1133, ButtonID::R3), 9 },

				{ std::make_tuple(1356, ButtonID::DOWN), 1 },
				{ std::make_tuple(1356, ButtonID::RIGHT), 2 },
				{ std::make_tuple(1356, ButtonID::LEFT), 0 },
				{ std::make_tuple(1356, ButtonID::UP), 3 },
				{ std::make_tuple(1356, ButtonID::LEFT_BUTTON), 4 },
				{ std::make_tuple(1356, ButtonID::RIGHT_BUTTON), 5 },
				{ std::make_tuple(1356, ButtonID::SELECT), 8 },
				{ std::make_tuple(1356, ButtonID::START), 9 },
				{ std::make_tuple(1156, ButtonID::L3), 10 },
				{ std::make_tuple(1156, ButtonID::R3), 11 },

				{ std::make_tuple(6708, ButtonID::DOWN), 1 },
				{ std::make_tuple(6708, ButtonID::RIGHT), 2 },
				{ std::make_tuple(6708, ButtonID::LEFT), 0 },
				{ std::make_tuple(6708, ButtonID::UP), 3 },
				{ std::make_tuple(6708, ButtonID::LEFT_BUTTON), 4 },
				{ std::make_tuple(6708, ButtonID::RIGHT_BUTTON), 5 },
				{ std::make_tuple(6708, ButtonID::SELECT), 8 },
				{ std::make_tuple(6708, ButtonID::START), 9 },
				{ std::make_tuple(6708, ButtonID::L3), 10 },
				{ std::make_tuple(6708, ButtonID::R3), 11 },

				{ std::make_tuple(1464, ButtonID::DOWN), 2 },
				{ std::make_tuple(1464, ButtonID::RIGHT), 3 },
				{ std::make_tuple(1464, ButtonID::LEFT), 0 },
				{ std::make_tuple(1464, ButtonID::UP), 1 },
				{ std::make_tuple(1464, ButtonID::LEFT_BUTTON), 4 },
				{ std::make_tuple(1464, ButtonID::RIGHT_BUTTON), 5 },
				{ std::make_tuple(1464, ButtonID::SELECT), 8 },
				{ std::make_tuple(1464, ButtonID::START), 9 },
				{ std::make_tuple(1464, ButtonID::L3), 10 },
				{ std::make_tuple(1464, ButtonID::R3), 11 },

				{ std::make_tuple(7640, ButtonID::DOWN), 2 },
				{ std::make_tuple(7640, ButtonID::RIGHT), 1 },
				{ std::make_tuple(7640, ButtonID::LEFT), 3 },
				{ std::make_tuple(7640, ButtonID::UP), 0 },
				{ std::make_tuple(7640, ButtonID::LEFT_BUTTON), 6 },
				{ std::make_tuple(7640, ButtonID::RIGHT_BUTTON), 7 },
				{ std::make_tuple(7640, ButtonID::SELECT), 8 },
				{ std::make_tuple(7640, ButtonID::START), 9 },
				{ std::make_tuple(7640, ButtonID::L3), 10 },
				{ std::make_tuple(7640, ButtonID::R3), 11 },

				{ std::make_tuple(1118, ButtonID::DOWN), 0 },
				{ std::make_tuple(1118, ButtonID::RIGHT), 1 },
				{ std::make_tuple(1118, ButtonID::LEFT), 2 },
				{ std::make_tuple(1118, ButtonID::UP), 3 },

				{ std::make_tuple(1118, ButtonID::START), 7 },
				{ std::make_tuple(1118, ButtonID::SELECT), 6 },
			};
			return gamepad_map.find(std::make_tuple(vender_id, button_id))->second;
		}

		int getVendorID(int device_id)const
		{
			return vendor_map.find(device_id)->second;
		}

		float getPressAxis(int device_id, AxisID axis_id)const
		{
			const auto& find = press_axis.find(std::make_tuple(device_id, getAxisID(getVendorID(device_id), axis_id)));
			if (find == press_axis.end())return 0.0f;
			return find->second;
		}

		bool isPushButton(int device_id,ButtonID button_id)const
		{
			const auto& find = push_button.find(std::make_tuple(device_id, getButtonID(getVendorID(device_id), button_id)));
			if (find == push_button.end())return false;
			return true;
		}
		bool isPressButton(int device_id, ButtonID button_id)const
		{
			const auto& find = press_button.find(std::make_tuple(device_id, getButtonID(getVendorID(device_id), button_id)));
			if (find == press_button.end())return false;
			return true;
		}

		bool isPullButton(int device_id, ButtonID button_id)const
		{
			const auto& find = pull_button.find(std::make_tuple(device_id, getButtonID(getVendorID(device_id), button_id)));
			if (find == pull_button.end())return false;
			return true;
		}

		void clear();

		static GamePad& getInstance();
	};
};