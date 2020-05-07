#pragma once
#include "PhysicsNode.h"
#include "MeshNode.h"
#include "Mesh.h"
#include "CameraNode.h"

/**
 * Borealis ship node for handling controls, physics, and camera.
 *
 * Note: design totally not shamelessly stolen from "Aurora". Any reference
 * or similarity is purely coincidental.
 *
 */
class BorealisNode : public PhysicsNode
{
public:
                            BorealisNode() : PhysicsNode(L"Borealis Ship")      {}
    virtual                ~BorealisNode()                                      {}

    virtual bool            Initialise() override;
    virtual void            Update(FXMMATRIX& m) override;

private:
    shared_ptr<Mesh>        _borealisModel;
    shared_ptr<MeshNode>    _borealisMeshNode;
    shared_ptr<CameraNode>  _camera;

    Vector3                 _pilotPosition{ 0, 1.f, 3.5f };

    float                   _accelerationSpeed{ .01f };
    float                   _rotationSpeed{ 1.f };
};

