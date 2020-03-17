#pragma once
#include "MeshNode.h"
#include "IPrimitive.h"

/**
 * Primitive cube shape.
 *
 */
class CubeNode : public MeshNode, public IPrimitive
{

public:
    /*     Constructors     */

    CubeNode(const wstring& name) : MeshNode(name) {}
    virtual bool Initialise() override;
};

