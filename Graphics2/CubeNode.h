#pragma once
#include "MeshNode.h"

/**
 * Represents a cube mesh.
 *
 */
class CubeNode : public MeshNode
{
public:
    /**     Constructors     */
    CubeNode()              : MeshNode(L"Cube Node")     {}
    CubeNode(wstring name)  : MeshNode(name)             {}

    virtual vector<Vertex>  MeshVertices()      override;
    virtual vector<UINT>    MeshIndices()       override;

    virtual bool            Initialise()        override;
};

