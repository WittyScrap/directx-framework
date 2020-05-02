#pragma once
#include "DirectXCore.h"


// Shorthand
#define X		GetX()
#define Y		GetY()
#define Z		GetZ()

/**
 * Represents a 3D vector.
 */
class Vector3
{
public:
    /* Constructors */
    Vector3();
    Vector3(const Vector3& copy);
    Vector3(const XMVECTOR& copy);
    Vector3(const XMFLOAT3& copy);
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

    void Normalize();
    const Vector3 Normalized() const;

    static const float Dot(const Vector3& lhs, const Vector3& rhs);
    static const float Dot(const XMVECTOR& lhs, const XMVECTOR& rhs);

    static const Vector3 Cross(const Vector3& lhs, const Vector3& rhs);
    static const XMVECTOR Cross(const XMVECTOR& lhs, const XMVECTOR& rhs);

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

    operator XMFLOAT3() { return XMFLOAT3(_x, _y, _z); }
    operator XMVECTOR() { return XMVectorSet(_x, _y, _z, 0); }

    XMFLOAT4 ToDX() const;
    XMFLOAT3 ToDX3() const;

    static const Vector3 UpVector;
    static const Vector3 DownVector;
    static const Vector3 LeftVector;
    static const Vector3 RightVector;
    static const Vector3 ForwardVector;
    static const Vector3 BackwardVector;

private:
    float _x;
    float _y;
    float _z;

};