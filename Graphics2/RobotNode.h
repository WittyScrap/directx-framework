#pragma once
#include "SceneGraph.h"

/**
 * Simple testing demonstration robot shape.
 *
 */
class RobotNode : public SceneGraph
{
public:
    RobotNode() : SceneGraph(L"Robot") {}
    RobotNode(wstring name) : SceneGraph(name) {}

    virtual ~RobotNode() {}

    virtual bool Initialise() override;
    virtual void Update() override;

    float RotationSpeed{ 1 };
};

