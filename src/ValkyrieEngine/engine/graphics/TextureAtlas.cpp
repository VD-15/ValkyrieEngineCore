#include "TextureAtlas.h"

#include "stb/stb_image.h"

using namespace vlk;

TextureAtlas::TextureAtlas()
{
	this->data = nullptr;
	this->width = 0;
	this->height = 0;
}

TextureAtlas::~TextureAtlas()
{
	stbi_image_free(data);
}

Boolean TextureAtlas::LoadFromFile(const std::string& path)
{
	//stbi_set_flip_vertically_on_load(true);
	data = stbi_load(path.c_str(), &width, &height, nullptr, 4);
	return static_cast<Boolean>(data);
}