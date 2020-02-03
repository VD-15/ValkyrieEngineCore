#pragma once

#include "ValkyrieEngine/core/Content.hpp"
#include "ValkyrieEngine/common/Vector.h"

namespace vlk
{
	enum class FilterMode
	{
		Nearest,
		Linear,
		Mipmap
	};

	class Texture2D final : public Content<Texture2D>
	{
		public:
		Texture2D(const std::string& name, const std::string& path) noexcept;
		~Texture2D();

		//Size of the image in pixels
		Point size;

		//Top left corner of the image, relative to the origin
		Vector2 topLeft;

		//Bottom right corner of the image, relative to the origin
		Vector2 bottomRight;

		//Number of color channels in the image ranging from 1..4
		Int numChannels;
		FilterMode filterMode;

		Byte* data;
	};
}