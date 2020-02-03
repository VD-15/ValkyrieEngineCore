#pragma once

#include "glm/glm.hpp"
#include"glm/gtx/rotate_vector.hpp"
#include"glm/gtx/vector_angle.hpp"

#include "ValkyrieEngine/core/ValkyrieEngine.h"

namespace vlk
{
	typedef glm::ivec2 Point;

	typedef glm::vec2 Vector2;
	typedef glm::vec3 Vector3;
	typedef glm::vec4 Vector4;
	typedef glm::vec4 Color;

	namespace Vector
	{
		constexpr Vector2 Zero2	(0.0f, 0.0f);
		constexpr Vector2 One2	(1.0f, 1.0f);
		constexpr Vector2 UnitX2(1.0f, 0.0f);
		constexpr Vector2 UnitY2(0.0f, 1.0f);

		constexpr Vector3 Zero3	(0.0f, 0.0f, 0.0f);
		constexpr Vector3 One3	(1.0f, 1.0f, 1.0f);
		constexpr Vector3 UnitX3(1.0f, 0.0f, 0.0f);
		constexpr Vector3 UnitY3(0.0f, 1.0f, 0.0f);
		constexpr Vector3 UnitZ3(0.0f, 0.0f, 1.0f);

		inline Vector2 Rotate(const Vector2& v, Float angle) { return glm::rotate(v, angle); }

		inline Vector3 RotateX(const Vector3& v, Float angle) { return glm::rotateX(v, angle); }
		inline Vector3 RotateY(const Vector3& v, Float angle) { return glm::rotateY(v, angle); }
		inline Vector3 RotateZ(const Vector3& v, Float angle) { return glm::rotateZ(v, angle); }

		inline Vector2 RotateAround(const Vector2& v, const Vector2& origin, Float angle)
		{
			Float sinA = std::sinf(angle);
			Float cosA = std::cosf(angle);
			Float diffX = v.x - origin.x;
			Float diffY = v.y - origin.y;

			return Vector2(	cosA * diffX - sinA * diffY + origin.x,
						   sinA * diffX + cosA * diffY + origin.y);
		}

		inline Vector2 Normalize(const Vector2& v) { return glm::normalize(v); }
		inline Float Magnitude(const Vector2& v) { return glm::length(v); }
		inline Float DotProduct(const Vector2& a, const Vector2& b) { return glm::dot(a, b); }
		inline Float AngleBetween(const Vector2& a, const Vector2& b) { return glm::angle(a, b); }
		inline Vector2 Perpendicular(const Vector2& v) { return Vector2(v.y, -v.x); }
	}

	namespace Colors
	{
		constexpr Color Red		(1.0f, 0.0f, 0.0f, 1.0f);
		constexpr Color Green	(0.0f, 1.0f, 0.0f, 1.0f);
		constexpr Color Blue	(0.0f, 0.0f, 1.0f, 1.0f);
		constexpr Color White	(1.0f, 1.0f, 1.0f, 1.0f);
		constexpr Color Black	(0.0f, 0.0f, 0.0f, 1.0f);
	}

	/*
	template <class T>
	class Pair final
	{
		public:
		constexpr Pair<T>()
		{
			this->x = T();
			this->y = T();
		}

		constexpr Pair<T>(T x, T y)
		{
			this->x = x;
			this->y = y;
		}

		constexpr Pair<T>(T arr[2])
		{
			this->x = arr[0];
			this->y = arr[1];
		}

		constexpr Pair<T>(const Pair<T>& other) = default;
		constexpr Pair<T>(Pair<T>&& other) noexcept = default;
		constexpr Pair<T>& operator=(const Pair<T>& other) = default;

		constexpr Boolean operator==(const Pair<T>& v) const	{ return this->x == v.x && this->y == v.y; }
		constexpr Boolean operator!=(const Pair<T>& v) const	{ return this->x != v.x || this->y != v.y; }
		
		union
		{
			T x;
			T a;
			T start;
		};

		union
		{
			T y;
			T b;
			T end;
		};
	};
	
	class Vector2 final
	{
		public:
		Vector2();
		Vector2(Int x, Int y);
		Vector2(Float x, Float y);
		Vector2(Float xy);

		Vector2(const Vector2& other);
		Vector2(Vector2&& other) noexcept;
		Vector2& operator=(const Vector2& other);

		virtual ~Vector2();

		Float x;
		Float y;

		inline Boolean operator==(const Vector2& v) const	{ return this->x == v.x && this->y == v.y; }
		inline Boolean operator!=(const Vector2& v) const	{ return this->x != v.x || this->y != v.y; }

		inline Vector2 operator+ (const Vector2& v) const	{ return Vector2(this->x + v.x, this->y + v.y); }
		inline Vector2 operator- (const Vector2& v) const	{ return Vector2(this->x - v.x, this->y - v.y); }
		inline Vector2 operator* (Float f) const			{ return Vector2(this->x * f, this->y * f); }
		inline Vector2 operator/ (Float f) const			{ return Vector2(this->x / f, this->y / f); }
		inline Vector2 operator* (const Vector2& v) const	{ return Vector2(this->x * v.x, this->y * v.y); }

		inline Vector2 operator-() const { return Vector2(-this->x, -this->y); }

		inline Vector2& operator+=(const Vector2& v)		{ this->x += v.x; this->y += v.y; return *this; }
		inline Vector2& operator-=(const Vector2& v)		{ this->x -= v.x; this->y -= v.y; return *this; }
		inline Vector2& operator*=(Float f)					{ this->x *= f; this->y *= f; return *this; }
		inline Vector2& operator/=(Float f)					{ this->x /= f; this->y /= f; return *this; }
		inline Vector2& operator*=(const Vector2& v)		{ this->x *= v.x; this->y *= v.y; return *this; }

		void Rotate(Float angle);
		void RotateAround(const Vector2& origin, Float angle);
		void Normalize();
		void Normalize(Float magnitude);
		Float Magnitude() const;
		Float DotProduct(const Vector2& v) const;
		Float GetAngleBetween(const Vector2& v) const;
		Float GetProjection(const Vector2& target) const;
		Vector2 Perpendicular() const;
		void Round();

		static Vector2 Rotate(const Vector2& vector, Float angle);
		static Vector2 RotateAround(const Vector2& vector, const Vector2& origin, Float angle);
		static Vector2 Normalize(const Vector2& vector);
		static Float Magnitude(const Vector2& vector);
		static Float DotProduct(const Vector2& a, const Vector2& b);
		static Float GetAngleBetween(const Vector2& a, const Vector2& b);
		static Float GetProjection(const Vector2& vector, const Vector2& target);
		static Vector2 Perpendicular(const Vector2& v);
		static Vector2 Round(const Vector2& v);

		static const Vector2 UP;
		static const Vector2 DOWN;
		static const Vector2 LEFT;
		static const Vector2 RIGHT;
		static const Vector2 ONE;
	};

	class Vector3 final
	{
		public:
		Vector3();
		Vector3(Float x, Float y, Float z);
		Vector3(const Vector2& v);
		Vector3(const Vector2& xy, Float z);

		Vector3(const Vector3& other);
		Vector3(Vector3&& other) noexcept;
		Vector3& operator=(const Vector3& other);

		virtual ~Vector3();

		Float x;
		Float y;
		Float z;

		//Convert this Vector3 to a Vector2, scaling by z
		//Use this when converting from a matrix multiplication
		inline Vector2 ToVector2s() const { return Vector2(this->x / this->z, this->y / this->z); }

		//Convert this Vector3 to a Vector2, discarding z
		inline Vector2 ToVector2d() const { return Vector2(this->x, this->y); }

		inline Boolean operator==(const Vector3& v) const { return this->x == v.x && this->y == v.y && this->z == v.z; }
		inline Boolean operator!=(const Vector3& v) const { return this->x != v.x || this->y != v.y || this->z != v.z; }

		inline Vector3 operator+ (const Vector3& v) const { return Vector3(this->x + v.x, this->y + v.y, this->z + v.z); }
		inline Vector3 operator- (const Vector3& v) const { return Vector3(this->x - v.x, this->y - v.y, this->z - v.z); }
		inline Vector3 operator* (const Float f) const { return Vector3(this->x * f, this->y * f, this->z * f); }
		inline Vector3 operator/ (const Float f) const { return Vector3(this->x / f, this->y / f, this->z / f); }

		inline Vector3& operator+=(const Vector3& v) { this->x += v.x; this->y += v.y; this->z += v.z; return *this; }
		inline Vector3& operator-=(const Vector3& v) { this->x -= v.x; this->y -= v.y; this->z -= v.z; return *this; }
		inline Vector3& operator*=(const Float f) { this->x *= f; this->y *= f; this->z *= f; return *this; }
		inline Vector3& operator/=(const Float f) { this->x /= f; this->y /= f; this->z /= f; return *this; }

		static const Vector3 UP;
		static const Vector3 DOWN;
		static const Vector3 LEFT;
		static const Vector3 RIGHT;
		static const Vector3 FORWARD;
		static const Vector3 BACKWARD;
		static const Vector3 ONE;
		static const Vector3 IDENTITY;
	};

	class Vector4 final
	{
		public:

		//Initializes a Vector4 at 0, 0, 0, 0
		Vector4();

		//Initializes a Vector4 from the given x/y/z components with a W-value of 1
		Vector4(Float x, Float y, Float z);

		//Initializes a Vector4 from the given x/y/z/w components
		Vector4(Float x, Float y, Float z, Float w);


		//Initializes a Vector4 from the given Vector2 with a Z-value of 0 and a W-value of 1
		Vector4(const Vector2& xy);

		//Initializes a Vector4 from the given Vector2 with the given Z-value and a W-value of 1
		Vector4(const Vector2& xy, Float z);

		//Initializes a Vector4 from the given Vector2 with the given Z-value and W-value
		Vector4(const Vector2& xy, Float z, Float w);

		//Initializes a Vector4 from the given Vector3 with a W-value of 1
		Vector4(const Vector3& xyz);

		//Initializes a Vector4 from the given Vector3 and W-value
		Vector4(const Vector3& xyz, Float w);

		Vector4(const Vector4& other);
		Vector4(Vector4&& other) noexcept;
		Vector4& operator=(const Vector4& other);

		virtual ~Vector4();

		Float x;
		Float y;
		Float z;
		Float w;

		//Convert this Vector4 to a Vector3, scaling by w
		//Use this when converting from a matrix multiplication
		inline Vector3 ToVector3s() const { return Vector3(this->x / this->w, this->y / this->w, this->z / this->w); }

		//Convert this Vector4 to a Vector3, discarding w
		inline Vector3 ToVector3d() const { return Vector3(this->x, this->y, this->z); }

		inline Boolean operator==(const Vector4& v) const { return this->x == v.x && this->y == v.y && this->z == v.z && this->w == v.w; }
		inline Boolean operator!=(const Vector4& v) const { return this->x != v.x || this->y != v.y || this->z != v.z && this->w == v.w; }

		inline Vector4 operator+ (const Vector4& v) const { return Vector4(this->x + v.x, this->y + v.y, this->z + v.z, this->w + v.w); }
		inline Vector4 operator- (const Vector4& v) const { return Vector4(this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w); }
		inline Vector4 operator* (const Float f) const { return Vector4(this->x * f, this->y * f, this->z * f, this->w * f); }
		inline Vector4 operator/ (const Float f) const { return Vector4(this->x / f, this->y / f, this->z / f, this->w / f); }

		inline Vector4& operator+=(const Vector4& v) { this->x += v.x; this->y += v.y; this->z += v.z; this->w += v.w; return *this; }
		inline Vector4& operator-=(const Vector4& v) { this->x -= v.x; this->y -= v.y; this->z -= v.z; this->w -= v.w; return *this; }
		inline Vector4& operator*=(const Float f) { this->x *= f; this->y *= f; this->z *= f; this->w *= f; return *this; }
		inline Vector4& operator/=(const Float f) { this->x /= f; this->y /= f; this->z /= f; this->w /= f; return *this; }
		
		static const Vector4 UP;
		static const Vector4 DOWN;
		static const Vector4 LEFT;
		static const Vector4 RIGHT;
		static const Vector4 FORWARD;
		static const Vector4 BACKWARD;
		static const Vector4 ONE;
		static const Vector4 IDENTITY;
	};
	*/
}