#pragma once

#include "../utils/NonClonable.h"
#include "../core/ValkyrieEngine.h"

namespace vlk
{
	class TextureAtlas : public NonClonable
	{
		public:
		TextureAtlas();
		virtual ~TextureAtlas();

		inline const Byte* const Data() const { return data; }
		inline const Int Width() const { return width; }
		inline const Int Height() const { return height; }

		Boolean LoadFromFile(const std::string& path);

		private:
		Byte* data;
		Int width;
		Int height;
	};
}