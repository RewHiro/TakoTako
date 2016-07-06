#pragma once

#include <unordered_map>

#include "boost/noncopyable.hpp"

#include "cinder/Font.h"
#include "cinder/gl/Texture.h"
#include "cinder/ObjLoader.h"

enum class TextureType
{
	BACKGROUND,
	GROUND,
	PLAYER01,
	PLAYER02,
	PLAYER03,
	PLAYER04,
	PLAYER_FOOT,
	UI_HIT_POINT,
	SEAWEED,
	ROCK01,
	ROCK02,
	ITEM01_BODY,
	ITEM01_ARM,
	METARIC,
	SUN,
	CORONA,
	PLAYER02_FOOT,
	PLAYER03_FOOT,
	PLAYER04_FOOT,
	BUBBLE,
	MANUAL,
	MANUAL01,
	MANUAL02,
	MANUAL03,
	MANUAL04,
	MANUAL05,
};

enum class ModelType
{
	PLAYER_BODY,
	PLAYER_FOOT,
	ROCK01,
	ROCK02,
	PLAYER_BULLET,
	ITEM01_BODY,
	ITEM01_ARM,
};

class ResourceManager : private boost::noncopyable
{
public:

	enum class FontType
	{
		MEIRYO,
		MEIRYO_BOLD,
	};

	~ResourceManager() = default;

	static ResourceManager& getInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	void add(const FontType font_type, const ci::Font& font);
	void add(const TextureType texture_type, const ci::gl::Texture& texture);
	void add(const ModelType model_type, const ci::ObjLoader obj_loader);

	ci::Font find(const FontType font_type)const;
	const ci::gl::Texture& find(const TextureType texture_type);
	ci::ObjLoader find(const ModelType model_type)const;

	void clear();

private:

	ResourceManager() = default;

	std::unordered_map<FontType, ci::Font>font_map_;
	std::unordered_map < TextureType, const ci::gl::Texture > texture_map_;
	std::unordered_map < ModelType, const ci::ObjLoader > model_map_;
};

using FontType = ResourceManager::FontType;