#pragma once
#include "PhysicsNode.h"
#include "MeshNode.h"

/**
 * Small satellite that can be set in orbit
 */
class SatelliteNode : public PhysicsNode
{
public:
                    SatelliteNode(wstring name) : PhysicsNode(name)         {}
    virtual        ~SatelliteNode()                                         {}

    virtual bool    Initialise() override;

private:
    shared_ptr<MeshNode> _satellite;
};

bool SatelliteNode::Initialise()
{
    _satellite = SceneGraph::Create<MeshNode>(L"SatelliteGeometry");
    _satellite->Build(RESOURCES->GetMesh(L"Models/Satellite/satellite.fbx"));
    Add(_satellite);

    return true;
}