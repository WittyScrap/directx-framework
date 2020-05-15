#pragma once
#include "MeshNode.h"
#include "CameraNode.h"


/**
 * Generates a skybox that follows the camera.
 *
 */
class SkyboxNode : public MeshNode
{
public:
                                SkyboxNode(wstring name) : MeshNode(name)           {}
                                SkyboxNode()             : MeshNode(L"Skybox")      {}
    virtual                    ~SkyboxNode()                                        {}

    virtual bool                Initialise() override;
    virtual void                Update() override;

            void                Link(const shared_ptr<CameraNode>& camera)          { _linkedCamera = camera; }

private:
    shared_ptr<Material>        _material;
    shared_ptr<CameraNode>      _linkedCamera;
};

