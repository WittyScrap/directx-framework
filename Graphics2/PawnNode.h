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

    return CameraNode::Initialise();
}

void PawnNode::Update(FXMMATRIX& m)
{
    Vector3 location = GetPosition();
    Vector3 rotation = GetRotation();

    float yaw = rotation.GetY();
    yaw += GetMouseHorizontal() * _rotationSpeed;
    rotation.SetY(yaw);

    SetRotation(rotation);

    location += GetForwardVector() * _movementSpeed * (GetKey('W') - GetKey('S'));
    location += GetRightVector() * _movementSpeed * (GetKey('D') - GetKey('A'));
    location += GetUpVector() * _movementSpeed * (GetKey('E') - GetKey('Q'));

    SetPosition(location);

    CameraNode::Update(m);

    if (!_mouseLocked && GetKeyDown(VK_RETURN))
    {
        SetMouseLocked(true);
    }
}