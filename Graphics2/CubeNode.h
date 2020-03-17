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

    CubeNode(wstring name) {}

    virtual bool Initialise() override;
};

