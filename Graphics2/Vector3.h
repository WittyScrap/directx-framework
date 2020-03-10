#pragma once
#include "DirectXCore.h"

/**
 * Represents a 3D vector.
 */
class Vector3
{
public:
    /* Constructors */
    Vector3();
    Vector3(const Vector3& copy);
    Vector3(const float& x, const float& y, const float& z);

    virtual ~Vector3();

    const float& GetX() const;
    const float& GetY() const;
    const float& GetZ() const;

    void SetX(const float& x);
    void SetY(const float& y);
    void SetZ(const float& z);

    const float Length() const;
    const float SqrLength() const;

    static const float Dot(const Vector3& lhs, const Vector3& rhs);
    static const Vector3 Cross(const Vector3& lhs, const Vector3& rhs);

    const Vector3 operator+(const Vector3& rhs) const;
    const Vector3 operator-(const Vector3& rhs) const;
    const Vector3 operator*(const Vector3& rhs) const;
    const Vector3 operator/(const Vector3& rhs) const;

    const Vector3& operator+=(const Vector3& rhs);
    const Vector3& operator-=(const Vector3& rhs);
    const Vector3& operator*=(const Vector3& rhs);
    const Vector3& operator/=(const Vector3& rhs);

    const Vector3 operator+(const float& rhs) const;
    const Vector3 operator-(const float& rhs) const;
    const Vector3 operator*(const float& rhs) const;
    const Vector3 operator/(const float& rhs) const;

    const Vector3& operator+=(const float& rhs);
    const Vector3& operator-=(const float& rhs);
    const Vector3& operator*=(const float& rhs);
    const Vector3& operator/=(const float& rhs);

    XMFLOAT3 ToDX() const;

private:
    float _x;
    float _y;
    float _z;

};