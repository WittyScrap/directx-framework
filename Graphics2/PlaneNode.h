#pragma once
#include "MeshNode.h"

/**
 * Plane mesh imported as a node object.
 *
 */
class PlaneNode : public MeshNode
{
public:
    PlaneNode() : MeshNode(L"Plane") {}
    PlaneNode(wstring name) : MeshNode(name) {}

    virtual ~PlaneNode() {}

    virtual bool Initialise() override;
    virtual void Update(FXMMATRIX& m) override;

private:

};

