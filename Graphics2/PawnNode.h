#pragma once
#include "CameraNode.h"

/**
 * A simple UE4-like default pawn that can fly and look around.
 *
 */
class PawnNode : public CameraNode
{
public:
                        PawnNode() : CameraNode(L"Default Pawn")            {}
    virtual            ~PawnNode()                                          {}

    inline  void        SetMovementSpeed(const FLOAT& movementSpeed)        { _movementSpeed = movementSpeed; }
    inline  void        SetRotationSpeed(const FLOAT& rotationSpeed)        { _rotationSpeed = rotationSpeed; }

    virtual bool        Initialise()         override;
    virtual void        Update(FXMMATRIX& m) override;

private:
        FLOAT           _movementSpeed{ 2.5f };
        FLOAT           _rotationSpeed{ 10.f };
};

inline bool PawnNode::Initialise()
{
    SetMouseLocked(true);
    SetMouseVisible(false);

    return CameraNode::Initialise();
}

void PawnNode::Update(FXMMATRIX& m)
{
    Vector3 location = GetPosition();
    Vector3 rotation = GetRotation();

    float pitch = rotation.GetX();
    float yaw = rotation.GetY();
    float roll = rotation.GetZ();

    pitch += GetMouseVertical() * _rotationSpeed;
    yaw += GetMouseHorizontal() * _rotationSpeed;
    roll += static_cast<float>(GetKey('E') - GetKey('Q')) * _rotationSpeed;

    rotation.SetX(pitch);
    rotation.SetY(yaw);
    rotation.SetZ(roll);

    SetRotation(rotation);

    location += GetForwardVector() * _movementSpeed * static_cast<float>(GetKey('W') - GetKey('S'));
    location += GetRightVector() * _movementSpeed * static_cast<float>(GetKey('D') - GetKey('A'));
    location += GetUpVector() * _movementSpeed * static_cast<float>(GetKey('R') - GetKey('F'));

    SetPosition(location);

    CameraNode::Update(m);

    if (!_mouseLocked && GetKeyDown(VK_LBUTTON))
    {
        SetMouseLocked(true);
        SetMouseVisible(false);
    }
}