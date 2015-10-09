#pragma once
#include "Component.h"

#include <Sphere.h>

typedef CU::Intersection::Sphere Sphere;

class CollisionComponent : public Component
{
public:
	CollisionComponent(Entity& aEntity);
	~CollisionComponent();
	void Initiate(float someRadius);
	void Update(float aDeltaTime) override;

	void SetRadius(float someRadius = 0);
	Sphere GetSphere();


	static eComponentType GetType();

private:
	Sphere mySphere;
};



inline eComponentType CollisionComponent::GetType()
{
	return eComponentType::COLLISION;
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