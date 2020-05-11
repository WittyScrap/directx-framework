#pragma once
#include "MeshNode.h"


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

private:
    shared_ptr<Material>        _material;
};

