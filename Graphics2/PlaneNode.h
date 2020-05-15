#pragma once
#include "MeshNode.h"

/**
 * Plane mesh imported as a node object.
 *
 */
class PlaneNode : public SceneGraph
{
public:
    PlaneNode() : SceneGraph(L"Plane") {}
    PlaneNode(wstring name) : SceneGraph(name) {}

    virtual ~PlaneNode() {}

    virtual bool Initialise() override;
    virtual void Update() override;

    float MovementSpeed{ 0.f };
    float TurningSpeed{ 0.25f };
    float BankingAngle{ 45.f };
    float PitchingAngle{ 10.f };

private:
    shared_ptr<SceneGraph> _planeController{ nullptr };
    float _turningAmount{ 0 };
};

