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
    virtual void Update(FXMMATRIX& m) override;

    float MovementSpeed{ 1.f };
    float TurningSpeed{ 0.25f };
};

