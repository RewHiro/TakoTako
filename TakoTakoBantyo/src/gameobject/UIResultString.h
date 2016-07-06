#pragma once

#include "GameObject.h"

class Player;

class UIResultString : public GameObject
{
public:

	UIResultString(const std::string& result_string);
	static std::shared_ptr<UIResultString>const create(const std::shared_ptr<Player>& player, const std::string& result_string);

private:

	void draw()final;

	ci::Font font_;
	ci::ColorA color_;
	std::string result_string_;
};