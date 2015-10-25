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

	void SetCollisionRadius(float someRadius = 0);
	Sphere GetSphere();

	bool CheckCollision(CollisionComponent* anOtherComponent);

	static eComponentType GetType();

private:
	Sphere mySphere;
};



inline eComponentType CollisionComponent::GetType()
{
	return eComponentType::COLLISION;
}

inline void CollisionComponent::SetCollisionRadius(float someRadius)
{
	mySphere.myRadius = someRadius;
	mySphere.myRadiusSquared = mySphere.myRadius * mySphere.myRadius;
}

inline Sphere CollisionComponent::GetSphere()
{
	return mySphere;
}