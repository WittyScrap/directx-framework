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
    virtual void        Update()             override;

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

void PawnNode::Update()
{
    Vector3 location = GetPosition();

    RotateAround(GetForwardVector(),    static_cast<float>(GetKey('Q') - GetKey('E')) * _rotationSpeed * .5f * FRAMEWORK->GetDeltaTime());
    RotateAround(GetUpVector(),         GetMouseHorizontal() * _rotationSpeed * FRAMEWORK->GetDeltaTime());
    RotateAround(GetRightVector(),      GetMouseVertical() * _rotationSpeed * FRAMEWORK->GetDeltaTime());

    location += GetForwardVector()  * _movementSpeed * static_cast<float>(GetKey('W') - GetKey('S') * FRAMEWORK->GetDeltaTime());
    location += GetRightVector()    * _movementSpeed * static_cast<float>(GetKey('D') - GetKey('A') * FRAMEWORK->GetDeltaTime());
    location += GetUpVector()       * _movementSpeed * static_cast<float>(GetKey('R') - GetKey('F') * FRAMEWORK->GetDeltaTime());

    SetPosition(location);

    CameraNode::Update();

    if (!_mouseLocked && GetKeyDown(VK_LBUTTON) && GetForegroundWindow() == FRAMEWORK->GetHWnd())
    {
        SetMouseLocked(true);
        SetMouseVisible(false);
    }
}