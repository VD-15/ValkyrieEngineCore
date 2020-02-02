#include "Matrix.h"

using namespace vlk;
/*
Matrix3::Matrix3() :
	data {}
{

}

Matrix3::Matrix3(const Matrix3& other) :
	data {}
{
	for (UInt i = 0; i < 9; i++)
	{
		this->data[i] = other.data[i];
	}
}

Matrix3::Matrix3(Matrix3&& other) noexcept :
	data {}
{
	for (UInt i = 0; i < 9; i++)
	{
		this->data[i] = other.data[i];
	}
}

Matrix3& Matrix3::operator=(const Matrix3& other)
{
	for (UInt i = 0; i < 9; i++)
	{
		this->data[i] = other.data[i];
	}

	return *this;
}

Matrix3::~Matrix3() {}

Boolean Matrix3::operator==(const Matrix3& other) const
{
	for (UInt i = 0; i < 9; i++)
	{
		if (data[i] != other.data[i])
		{
			return false;
		}
	}

	return true;
}

Boolean Matrix3::operator!=(const Matrix3& other) const
{
	for (UInt i = 0; i < 9; i++)
	{
		if (data[i] != other.data[i])
		{
			return true;
		}
	}

	return false;
}

Matrix3 Matrix3::operator*(const Matrix3& other) const
{
	Matrix3 m;

	for (UInt i = 0; i < 3; i++)
	{
		for (UInt j = 0; j < 3; j++)
		{
			for (UInt k = 0; k < 3; k++)
			{
				m.At(i, j) += this->At(k, j) * other.At(i, k);
			}
		}
	}

	return m;
}

Matrix3& Matrix3::operator*=(const Matrix3& other)
{
	Matrix3 m;

	for (UInt i = 0; i < 3; i++)
	{
		for (UInt j = 0; j < 3; j++)
		{
			for (UInt k = 0; k < 3; k++)
			{
				m.At(i, j) += this->At(k, j) * other.At(i, k);
			}
		}
	}

	*this = m;

	return *this;
}

Matrix3 Matrix3::operator*(const Float scale) const
{
	Matrix3 m;

	for (UInt i = 0; i < 9; i++)
	{
		m.data[i] = this->data[i] * scale;
	}

	return m;
}

Matrix3& Matrix3::operator*=(const Float scale)
{
	Matrix3 m;

	for (UInt i = 0; i < 9; i++)
	{
		m.data[i] = this->data[i] * scale;
	}

	*this = m;
	return *this;
}

Vector3 Matrix3::operator*(const Vector3& v) const
{
	Vector3 vec;

	vec.x += this->At(0, 0) * v.x;
	vec.x += this->At(1, 0) * v.y;
	vec.x += this->At(2, 0) * v.z;

	vec.y += this->At(0, 1) * v.x;
	vec.y += this->At(1, 1) * v.y;
	vec.y += this->At(2, 1) * v.z;

	vec.z += this->At(0, 2) * v.x;
	vec.z += this->At(1, 2) * v.y;
	vec.z += this->At(2, 2) * v.z;

	return vec;
}

Matrix3 Matrix3::CreateIdentity()
{
	Matrix3 m;

	for (UInt i = 0; i < 3; i++)
	{
		m.At(i, i) = 1.0f;
	}

	return m;
}

Matrix3 Matrix3::CreateTranslation(const Vector2& translation)
{
	Matrix3 m(Matrix3::CreateIdentity());

	m.At(2, 0) = translation.x;
	m.At(2, 1) = translation.y;

	return m;
}

Matrix3 Matrix3::CreateScale(const Vector2& scale)
{
	Matrix3 m;

	m.At(0, 0) = scale.x;
	m.At(1, 1) = scale.y;
	m.At(2, 2) = 1.0f;

	return m;
}

Matrix3 Matrix3::CreateRotation(const Float angle)
{
	Matrix3 m;

	Float cosa = cosf(angle);
	Float sina = sinf(angle);

	m.At(0, 0) = cosa;
	m.At(1, 0) = -sina;
	m.At(0, 1) = sina;
	m.At(1, 1) = cosa;
	m.At(2, 2) = 1.0f;

	return m;
}

Matrix4::Matrix4() :
	data {0.0f}
{

}

Matrix4::Matrix4(const Matrix4& other) :
	data {}
{
	for (UInt i = 0; i < 16; i++)
	{
		data[i] = other.data[i];
	}
}

Matrix4::Matrix4(Matrix4&& other) noexcept :
	data {}
{
	for (UInt i = 0; i < 16; i++)
	{
		data[i] = other.data[i];
	}
}

Matrix4& Matrix4::operator=(const Matrix4& other)
{
	for (UInt i = 0; i < 16; i++)
	{
		data[i] = other.data[i];
	}

	return *this;
}

Matrix4::~Matrix4() {}

Boolean Matrix4::operator==(const Matrix4& other) const
{
	for (UInt i = 0; i < 16; i++)
	{
		if (data[i] != other.data[i])
		{
			return false;
		}
	}

	return true;
}

Boolean Matrix4::operator!=(const Matrix4& other) const
{
	for (UInt i = 0; i < 16; i++)
	{
		if (data[i] != other.data[i])
		{
			return true;
		}
	}

	return false;
}

Matrix4 Matrix4::operator*(const Matrix4& other) const
{
	Matrix4 m;

	for (UInt i = 0; i < 4; i++)
	{
		for (UInt j = 0; j < 4; j++)
		{
			m.At(i, j) = 0.0f;

			for (UInt k = 0; k < 4; k++)
			{
				m.At(i, j) += this->At(k, j) * other.At(i, k);
			}
		}
	}

	return m;
}

Matrix4& Matrix4::operator*=(const Matrix4& other)
{
	Matrix4 m;

	for (UInt i = 0; i < 4; i++)
	{
		for (UInt j = 0; j < 4; j++)
		{
			m.At(i, j) = 0.0f;

			for (UInt k = 0; k < 4; k++)
			{
				m.At(i, j) += this->At(k, j) * other.At(i, k);
			}
		}
	}

	*this = m;
	return *this;
}

Matrix4 Matrix4::operator*(Float scale) const
{
	Matrix4 m;

	for (UInt i = 0; i < 16; i++)
	{
		m.data[i] = this->data[i] * scale;
	}

	return m;
}

Matrix4& Matrix4::operator*=(Float scale)
{
	for (UInt i = 0; i < 16; i++)
	{
		this->data[i] *= scale;
	}

	return *this;
}

Vector4 Matrix4::operator*(const Vector4& v) const
{
	Vector4 vec;

	vec.x += this->At(0, 0) * v.x;
	vec.x += this->At(1, 0) * v.y;
	vec.x += this->At(2, 0) * v.z;
	vec.x += this->At(3, 0) * v.w;

	vec.y += this->At(0, 1) * v.x;
	vec.y += this->At(1, 1) * v.y;
	vec.y += this->At(2, 1) * v.z;
	vec.y += this->At(3, 1) * v.w;

	vec.z += this->At(0, 2) * v.x;
	vec.z += this->At(1, 2) * v.y;
	vec.z += this->At(2, 2) * v.z;
	vec.z += this->At(3, 2) * v.w;

	vec.w += this->At(0, 3) * v.x;
	vec.w += this->At(1, 3) * v.y;
	vec.w += this->At(2, 3) * v.z;
	vec.w += this->At(3, 3) * v.w;

	return vec;
}

Matrix4 Matrix4::CreateIdentity()
{
	Matrix4 m;

	for (UInt i = 0; i < 4; i++)
	{
		m.At(i, i) = 1.0f;
	}

	return m;
}

Matrix4 Matrix4::CreateTranslation(const Vector3& translation)
{
	Matrix4 m(Matrix4::CreateIdentity());

	m.At(3, 0) = translation.x;
	m.At(3, 1) = translation.y;
	m.At(3, 2) = translation.z;
	m.At(3, 3) = 1.0f;

	return m;
}

Matrix4 Matrix4::CreateScale(const Vector3& scale)
{
	Matrix4 m;

	m.At(0, 0) = scale.x;
	m.At(1, 1) = scale.y;
	m.At(2, 2) = scale.z;
	m.At(3, 3) = 1.0f;

	return m;
}

Matrix4 Matrix4::CreateRotationX(Float angle)
{
	Matrix4 m;

	Float cosa = cosf(angle);
	Float sina = sinf(angle);

	m.At(0, 0) = 1.0f;
	m.At(1, 1) = cosa;
	m.At(2, 1) = -sina;
	m.At(1, 2) = sina;
	m.At(2, 2) = cosa;
	m.At(3, 3) = 1.0f;

	return m;
}

Matrix4 Matrix4::CreateRotationY(Float angle)
{
	Matrix4 m;

	Float cosa = cosf(angle);
	Float sina = sinf(angle);

	m.At(0, 0) = cosa;
	m.At(2, 0) = sina;
	m.At(0, 2) = -sina;
	m.At(2, 2) = cosa;
	m.At(1, 1) = 1.0f;
	m.At(3, 3) = 1.0f;

	return m;
}

Matrix4 Matrix4::CreateRotationZ(Float angle)
{
	Matrix4 m;

	Float cosa = cosf(angle);
	Float sina = sinf(angle);

	m.At(0, 0) = cosa;
	m.At(1, 0) = -sina;
	m.At(0, 1) = sina;
	m.At(1, 1) = cosa;
	m.At(2, 2) = 1.0f;
	m.At(3, 3) = 1.0f;

	return m;
}

Matrix4 Matrix4::CreateLookAt(Vector3 right, Vector3 up, Vector3 direction, Vector3 position)
{
	Matrix4 m(Matrix4::CreateIdentity());
	Matrix4 n(Matrix4::CreateIdentity());

	m.At(0, 0) = right.x;
	m.At(1, 0) = right.y;
	m.At(2, 0) = right.z;

	m.At(0, 1) = up.x;
	m.At(1, 1) = up.y;
	m.At(2, 1) = up.z;

	m.At(0, 2) = direction.x;
	m.At(1, 2) = direction.y;
	m.At(2, 2) = direction.z;

	n.At(0, 3) = -position.x;
	n.At(1, 3) = -position.y;
	n.At(2, 3) = -position.z;

	return m * n;
}

Matrix4 Matrix4::CreatePerspective(Float left, Float right, Float top, Float bottom, Float near, Float far)
{
	Matrix4 m;

	m.At(0, 0) = (2.0f * near) / (right - left);
	m.At(2, 0) = (right + left) / (right - left);
	m.At(1, 1) = (2.0f * near) / (top - bottom);
	m.At(2, 1) = (top + bottom) / (top - bottom);
	m.At(2, 2) = (-far - near) / (far - near);
	m.At(3, 2) = (-2.0f * far * near) / (far - near);
	m.At(2, 3) = -1.0f;

	return m;
}

Matrix4 Matrix4::CreateOrthographic(Float right, Float left, Float top, Float bottom, Float near, Float far)
{
	Matrix4 m;

	m.At(0, 0) = (2.0f) / (right - left);
	m.At(3, 0) = (-right - left) / (right - left);
	m.At(1, 1) = (2.0f) / (top - bottom);
	m.At(3, 1) = (-top - bottom) / (top - bottom);
	m.At(2, 2) = (-2.0f) / (far - near);
	m.At(3, 2) = (-far - near) / (far - near);
	m.At(3, 3) = 1.0f;

	return m;
}*/