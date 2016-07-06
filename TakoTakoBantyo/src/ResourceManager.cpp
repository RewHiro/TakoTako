#include "ResourceManager.h"


void ResourceManager::add(const FontType font_type, const ci::Font& font)
{
	font_map_.emplace(font_type, font);
}

void ResourceManager::add(const TextureType texture_type, const ci::gl::Texture& texture)
{
	texture_map_.emplace(texture_type, texture);
}

void ResourceManager::add(const ModelType model_type, const ci::ObjLoader obj_loader)
{
	model_map_.emplace(model_type, obj_loader);
}

ci::Font ResourceManager::find(const FontType font_type)const
{
	return font_map_.find(font_type)->second;
}

const ci::gl::Texture& ResourceManager::find(const TextureType texture_type)
{
	return texture_map_.find(texture_type)->second;
}

ci::ObjLoader ResourceManager::find(const ModelType model_type)const
{
	return model_map_.find(model_type)->second;
}

void ResourceManager::clear()
{
	font_map_.clear();
}