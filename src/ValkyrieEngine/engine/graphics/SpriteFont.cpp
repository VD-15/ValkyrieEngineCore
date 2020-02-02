#include "SpriteFont.h"
#include "utf8/utf8.h"

#include "stb/stb_image.h"

#include <fstream>

using namespace vlk;
/*
SpriteFont::SpriteFont()
{
	defaultChar = nullptr;
}

SpriteFont::~SpriteFont()
{
	for (auto it = encodings.begin(); it != encodings.end(); it++)
	{
		delete it->second;
	}
}

Int SpriteFont::GetWidth(const std::string& str) const
{
	#ifdef _DEBUG
	if (!utf8::is_valid(str.begin(), str.end()))
	{
		LogError("SpriteFont", "String passed was not valid UTF-8!");
		throw std::exception("String passed was not valid UTF-8!");
		return 0;
	}
	#endif

	Int width = 0;

	auto s = str.begin();
	while (s != str.end())
	{
		#ifdef _DEBUG
		UInt c = utf8::next(s, str.end());
		#else
		UInt c = utf8::unchecked::next(s);
		#endif

		auto it = encodings.find(c);

		if (it != encodings.end())
		{
			width += static_cast<Int>(std::round((it->second->size.x - it->second->origin.x) * this->Width()));
		}
		else
		{
			width += static_cast<Int>(std::round((defaultChar->size.x - defaultChar->origin.x) * this->Width()));
		}

		width++;
	}

	if (width > 0) width--;

	return width;
}

Int SpriteFont::GetFontHeight() const
{
	return static_cast<Int>(defaultChar->size.y * this->Height());
}

const Texture2D* SpriteFont::GetChar(UInt encoding) const
{
	return encodings.at(encoding);
}

void SpriteFont::AddCharEncoding(UInt encoding, const Texture2D* texture)
{
	#ifdef  _DEBUG
	if (texture->atlas != dynamic_cast<TextureAtlas*>(this))
	{
		LogError("SpriteFont", "Texture Atlases did not match.");
		throw std::exception("Texture Atlases did not match did not match.");
		return;
	}

	if (auto it = encodings.find(encoding); it != encodings.end())
	{
		LogWarning("LogError", "Reassigning font encoding.");
	}
	#endif

	encodings.insert_or_assign(encoding, texture);
}

void SpriteFont::SetDefaultChar(UInt encoding)
{
	if (auto it = encodings.find(encoding); it != encodings.end())
	{
		defaultChar = it->second;
	}
	else
	{
		LogError("SpriteFont", "Failed to find codepoint: " + encoding);
		throw std::exception("Failed to find codepoint");
	}
}*/