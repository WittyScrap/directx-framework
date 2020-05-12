#pragma once
#include "PlanetNode.h"

/**
 * Represents an object that can orbit a planet or system.
 *
 */
template<typename TBase, typename TExtend>
class Orbitable : public TExtend
{
public:
	Orbitable(wstring name) : TExtend(name) {}
	virtual ~Orbitable() { }
	void Orbit(PlanetNode* target);
};

template<typename TBase, typename TExtend>
inline void Orbitable<TBase, TExtend>::Orbit(PlanetNode* target)
{
	Vector3 objectToPlanet = target->GetWorldPosition() - dynamic_cast<TBase*>(this)->GetWorldPosition();
	Vector3 tangentVector = Vector3::Cross(target->GetUpVector(), objectToPlanet).Normalized();
	dynamic_cast<TBase*>(this)->SetLinearVelocity(tangentVector * target->GetOrbitalVelocity(objectToPlanet.Length()) + target->GetLinearVelocity());
}
