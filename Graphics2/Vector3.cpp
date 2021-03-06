#include "Vector3.h"

/* Constant vector definitions */

const Vector3 Vector3::UpVector			(0, 1, 0);
const Vector3 Vector3::DownVector		(0, -1, 0);
const Vector3 Vector3::LeftVector		(-1, 0, 0);
const Vector3 Vector3::RightVector		(1, 0, 0);
const Vector3 Vector3::ForwardVector	(0, 0, 1);
const Vector3 Vector3::BackwardVector	(0, 0, -1);
const Vector3 Vector3::ZeroVector		(0, 0, 0);
const Vector3 Vector3::OneVector		(1, 1, 1);

Vector3::Vector3() : _x{ 0 }, _y{ 0 }, _z{ 0 }
{ }

Vector3::Vector3(const Vector3& copy) : _x{ copy._x }, _y{ copy._y }, _z{ copy._z }
{ }

Vector3::Vector3(const XMVECTOR& copy)
{
	XMFLOAT3 o;
	XMStoreFloat3(&o, copy);

	_x = o.x; _y = o.y; _z = o.z;
}

Vector3::Vector3(const XMFLOAT3& copy) : _x{ copy.x }, _y{ copy.y }, _z{ copy.z }
{ }

Vector3::Vector3(const float& x, const float& y, const float& z) : _x{ x }, _y{ y }, _z{ z }
{ }

Vector3::~Vector3()
{ }

const float& Vector3::GetX() const
{
	return _x;
}

const float& Vector3::GetY() const
{
	return _y;
}

const float& Vector3::GetZ() const
{
	return _z;
}

void Vector3::SetX(const float& x)
{
	_x = x;
}

void Vector3::SetY(const float& y)
{
	_y = y;
}

void Vector3::SetZ(const float& z)
{
	_z = z;
}

const float Vector3::Length() const
{
	return sqrtf(SqrLength());
}

const float Vector3::SqrLength() const
{
	return (_x * _x) + (_y * _y) + (_z * _z);
}

void Vector3::Normalize()
{
	const float& length = Length();

	_x /= length;
	_y /= length;
	_z /= length;
}

const Vector3 Vector3::Normalized() const
{
	return Vector3(*this / Length());
}

const float Vector3::Dot(const Vector3& lhs, const Vector3& rhs)
{
	return (lhs._x * rhs._x) + (lhs._y * rhs._y) + (lhs._z * rhs._z);
}

const float Vector3::Dot(const XMVECTOR& lhs, const XMVECTOR& rhs)
{
	XMFLOAT2 o;

	XMVECTOR dot = XMVector3Dot(lhs, rhs);
	XMStoreFloat2(&o, dot);

	return o.x;
}

const Vector3 Vector3::Cross(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(lhs._y * rhs._z - lhs._z * rhs._y,
		lhs._z * rhs._x - lhs._x * rhs._z,
		lhs._x * rhs._y - lhs._y * rhs._x);
}

const XMVECTOR Vector3::Cross(const XMVECTOR& lhs, const XMVECTOR& rhs)
{
	return XMVector3Cross(lhs, rhs);
}

const Vector3& Vector3::Min(const Vector3& lhs, const Vector3& rhs)
{
	if (lhs.SqrLength() < rhs.SqrLength())
	{
		return lhs;
	}
	else
	{
		return rhs;
	}
}

const Vector3& Vector3::Max(const Vector3& lhs, const Vector3& rhs)
{
	if (rhs.SqrLength() > rhs.SqrLength())
	{
		return lhs;
	}
	else
	{
		return rhs;
	}
}

const Vector3 Vector3::operator+(const Vector3& rhs) const
{
	return Vector3(_x + rhs._x, _y + rhs._y, _z + rhs._z);
}

const Vector3 Vector3::operator-(const Vector3& rhs) const
{
	return Vector3(_x - rhs._x, _y - rhs._y, _z - rhs._z);
}

const Vector3 Vector3::operator*(const Vector3& rhs) const
{
	return Vector3(_x * rhs._x, _y * rhs._y, _z * rhs._z);
}

const Vector3 Vector3::operator/(const Vector3& rhs) const
{
	return Vector3(_x / rhs._x, _y / rhs._y, _z / rhs._z);
}

const Vector3& Vector3::operator+=(const Vector3& rhs)
{
	_x += rhs._x;
	_y += rhs._y;
	_z += rhs._z;

	return *this;
}

const Vector3& Vector3::operator-=(const Vector3& rhs)
{
	_x -= rhs._x;
	_y -= rhs._y;
	_z -= rhs._z;

	return *this;
}

const Vector3& Vector3::operator*=(const Vector3& rhs)
{
	_x *= rhs._x;
	_y *= rhs._y;
	_z *= rhs._z;

	return *this;
}

const Vector3& Vector3::operator/=(const Vector3& rhs)
{
	_x /= rhs._x;
	_y /= rhs._y;
	_z /= rhs._z;

	return *this;
}

const Vector3 Vector3::operator+(const float& rhs) const
{
	return Vector3(_x + rhs, _y + rhs, _z + rhs);
}

const Vector3 Vector3::operator-(const float& rhs) const
{
	return Vector3(_x - rhs, _y - rhs, _z - rhs);
}

const Vector3 Vector3::operator*(const float& rhs) const
{
	return Vector3(_x * rhs, _y * rhs, _z * rhs);
}

const Vector3 Vector3::operator/(const float& rhs) const
{
	return Vector3(_x / rhs, _y / rhs, _z / rhs);
}

const Vector3& Vector3::operator+=(const float& rhs)
{
	_x += rhs;
	_y += rhs;
	_z += rhs;

	return *this;
}

const Vector3& Vector3::operator-=(const float& rhs)
{
	_x -= rhs;
	_y -= rhs;
	_z -= rhs;

	return *this;
}

const Vector3& Vector3::operator*=(const float& rhs)
{
	_x *= rhs;
	_y *= rhs;
	_z *= rhs;

	return *this;
}

const Vector3& Vector3::operator/=(const float& rhs)
{
	_x /= rhs;
	_y /= rhs;
	_z /= rhs;

	return *this;
}

XMFLOAT4 Vector3::ToDX() const
{
	return XMFLOAT4(_x, _y, _z, 1);
}

XMFLOAT3 Vector3::ToDX3() const
{
	return XMFLOAT3(_x, _y, _z);
}
