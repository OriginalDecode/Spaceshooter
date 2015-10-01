#pragma once
#include "Component.h"

#include <Sphere.h>

typedef CU::Intersection::Sphere Sphere;
namespace Prism
{
	class Instance;
	class Camera;
}


class CollisionComponent : public Component
{
public:
	CollisionComponent(Entity& aEntity);
	~CollisionComponent();
	void Initiate(float someRadius);
	void Update(float aDeltaTime) override;

	void ReceiveMessage(const ShootMessage& aMessage) override;

	void SetRadius(float someRadius = 0);
	Sphere GetSphere();

	void Render(Prism::Camera* aCamera);



	static int GetID();

private:

	bool myShouldShow;

	Sphere mySphere;
	
	Prism::Instance* myInstance;

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