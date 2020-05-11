#pragma once
#include "PlanetNode.h"

/**
 * Represents an object that can orbit a planet or system.
 *
 */
template<typename TBase>
class Orbitable
{
public:
	virtual ~Orbitable() { }
	void Orbit(PlanetNode* target);
};

template<typename TBase>
inline void Orbitable<TBase>::Orbit(PlanetNode* target)
{
	Vector3 objectToPlanet = target->GetWorldPosition() - dynamic_cast<TBase*>(this)->GetWorldPosition();
	Vector3 tangentVector = Vector3::Cross(target->GetUpVector(), objectToPlanet).Normalized();
	dynamic_cast<TBase*>(this)->SetLinearVelocity(tangentVector * target->GetOrbitalVelocity(objectToPlanet.Length()));
}
