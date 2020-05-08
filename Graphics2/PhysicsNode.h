#pragma once
#include "SceneGraph.h"


/**
 * Handles a simplistic physics system.
 *
 */
class PhysicsNode :	public SceneGraph
{
public:
                            PhysicsNode() : PhysicsNode(L"Physics Node")        {}
                            PhysicsNode(wstring name) : SceneGraph(name)        {}

    virtual                ~PhysicsNode()                                       {}

            void            AddForce(const Vector3& force);
            void            AddSpin(const Vector3& spin);
            
            const Vector3   GetLinearVelocity() const;
            const Vector3   GetAngularVelocity() const;

            void            SetLinearVelocity(const Vector3& velocity);
            void            SetAngularVelocity(const Vector3& angularVelocity);

    virtual void            Update(FXMMATRIX& m) override;

private:
    Vector3                 _linearVelocity{ 0, 0, 0 };
    Vector3                 _angularVelocity{ 0, 0, 0 };
};

