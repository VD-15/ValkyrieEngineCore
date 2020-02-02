#include "Texture.h"

#include "stb/stb_image.h"
#include "nlohmann/json.hpp"

#include <fstream>

using namespace vlk;
using json = nlohmann::json;

namespace
{
	template <typename T>
	struct DefaultVal
	{
		static constexpr T Value()
		{
			if constexpr (std::numeric_limits<T>::has_infinity)
			{
				return std::numeric_limits<T>::infinity();
			}
			else
			{
				return std::numeric_limits<T>::min();
			}
		}
	};

	//constexpr Int defaultVal = std::numeric_limits<Int>::min();
}

Texture2D::Texture2D(const std::string& name, const std::string& path) noexcept:
	size(DefaultVal<Int>::Value()),
	topLeft(),
	bottomRight()
{
	Point location(DefaultVal<Int>::Value());
	Vector2 origin(DefaultVal<Float>::Value());

	if (std::ifstream metaIn(path + ".meta"); metaIn)
	{
		json j;
		metaIn >> j;
		metaIn.close();

		if (auto meta = j.find(name); meta != j.end())
		{
			if (auto iCrop = meta->find("crop"); iCrop != meta->end())
			{
				location.x = iCrop->value("x", DefaultVal<Int>::Value());
				location.y =iCrop->value("y", DefaultVal<Int>::Value());
				this->size.x = iCrop->value("w", DefaultVal<Int>::Value());
				this->size.y = iCrop->value("h", DefaultVal<Int>::Value());
			}

			if (auto iOrigin = meta->find("origin"); iOrigin != meta->end())
			{
				origin.x = iOrigin->value("x", DefaultVal<Float>::Value());
				origin.y = iOrigin->value("y", DefaultVal<Float>::Value());
			}

			this->numChannels = static_cast<Byte>(meta->value("channels", 4));

			std::string filter = meta->value("filter", "nearest");

			if (filter == "linear") this->filterMode = FilterMode::Linear;
			else if (filter == "mipmap") this->filterMode = FilterMode::Mipmap;
			else this->filterMode = FilterMode::Nearest;
		}
	}
	else
	{
		LogVerbose("Texture2D", "Could not find metadata file for " + path + " continuing with defaults.");
	}

	{
		this->numChannels = std::clamp(this->numChannels, 0, 4);

		Int srcWidth, srcHeight, srcChannels;

		stbi_set_flip_vertically_on_load(true);
		Byte* srcImage = stbi_load(path.c_str(), &srcWidth, &srcHeight, &srcChannels, this->numChannels);

		if (!srcImage)
		{
			LogError("Texture2D", "Failed to load image: " + path);
			return;
		}

		location.x = std::clamp(location.x, 0, srcWidth - 1);
		location.y = std::clamp(location.y, 0, srcHeight - 1); 

		this->size.x = std::clamp(this->size.x, 1, srcWidth - location.x);
		this->size.y = std::clamp(this->size.y, 1, srcHeight - location.y);

		if (origin.x == DefaultVal<Float>::Value()) origin.x = static_cast<Float>(this->size.x) / 2.0f;
		if (origin.y == DefaultVal<Float>::Value()) origin.y = static_cast<Float>(this->size.y) / 2.0f;

		stbi_image_free(srcImage);
	}
}

Texture2D::~Texture2D()
{

}