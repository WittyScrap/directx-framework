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

protected:
            void            DoCameraSway();
            void            RegisterThrust(const int& x, const int& y, const int& z);

private:
    shared_ptr<Mesh>        _borealisModel;
    shared_ptr<MeshNode>    _borealisMeshNode;
    shared_ptr<CameraNode>  _camera;

    Vector3                 _pilotPosition{ 0, 1.f, 3.5f };
    Vector3                 _lastThrust{ 0, 0, 0 };

    float                   _accelerationSpeed{ .25f };
    float                   _rotationSpeed{ 5.f };

    float                   _cameraSwayStrength{ .12f };
    float                   _cameraSwaySmoothness{ .05f };
    float                   _cameraBobStrength{ .025f };
    float                   _cameraHeadlookSensitivity{ 1024.f };

    float                   _cameraBobSpeed{ .01f };
    float                   _time{ 0 };

    float                   _minimumSpeed{ .05f };
    bool                    b_headlookActive{ false };
};

