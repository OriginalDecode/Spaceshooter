#pragma once
#include "Component.h"

#include <Sphere.h>
typedef CU::Intersection::Sphere Sphere;

class CollisionComponent : public Component
{
public:
	
	void Init() override;
	
	void Update(float aDeltaTime) override;
	
	void ReceiveMessage(const ShootMessage& aMessage) override;

	void SetRadius(float someRadius = 0);
	Sphere GetSphere();


	static int GetID();

private:

	Sphere mySphere;

};



inline int CollisionComponent::GetID()
{
	return 7;
}

inline void CollisionComponent::SetRadius(float someRadius)
{
	mySphere.myRadius = someRadius;
	mySphere.myRadiusSquared = mySphere.myRadius * mySphere.myRadius;
}

inline Sphere CollisionComponent::GetSphere()
{
	return mySphere;
}