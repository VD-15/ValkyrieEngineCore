#pragma once

#include "../core/ValkyrieEngine.h"
/*
namespace vlk
{
	struct Color final
	{
		Color();
		Color(Byte r, Byte g, Byte b, Byte a);
		Color(Float r, Float g, Float b, Float a);
		Color(UInt rgba);

		Float r;
		Float g;
		Float b;
		Float a;

		inline Boolean operator==(const Color& c) const
		{
			return this->r == c.r &&
				this->g == c.g &&
				this->b == c.b &&
				this->a == c.a;
		}

		inline Color operator+(const Color& c) const 
		{
			return Color(this->r + c.r, this->g + c.g, this->b + c.b, this->a + c.a);
		}

		inline Color operator*(const Color & c) const
		{
			return Color(this->r * c.r, this->g * c.g, this->b * c.b, this->a * c.a);
		}

		inline Color operator*(const Float f) const
		{
			return Color(this->r, this->g, this->b, this->a * f);
		}

		inline Color& operator+=(const Color& c)
		{
			this->r += c.r;
			this->g += c.g;
			this->b += c.b;
			this->a += c.a;
			return *this;
		}

		inline Color& operator*=(const Color& c)
		{
			this->r *= c.r;
			this->g *= c.g;
			this->b *= c.b;
			this->a *= c.a;
			return *this;
		}

		inline Color& operator*=(const Float f)
		{
			this->a *= f;
			return *this;
		}

		static const Color RED;
		static const Color GREEN;
		static const Color BLUE;
		static const Color ALPHA;
		static const Color WHITE;
		static const Color BLACK;
	};
}*/