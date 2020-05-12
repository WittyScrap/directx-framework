#pragma once
#include "SceneGraph.h"
#include "Orbitable.h"


/**
 * Handles a simplistic physics system.
 *
 */
class PhysicsNode :	public SceneGraph, public Orbitable<PhysicsNode>
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

    virtual void            Update() override;

            void            SetSimulateGravity(const BOOL& bValue)              { b_simulateGravity = bValue; }
            const BOOL&     GetSimulateGravity() const                          { return b_simulateGravity; }

            const Vector3   GetCollisionOffset(const PlanetNode* planet);

private:
    Vector3                 _linearVelocity{ 0, 0, 0 };
    Vector3                 _angularVelocity{ 0, 0, 0 };

    BOOL                    b_simulateGravity{ true };
    FLOAT                   _mass{ 1.f };
};

