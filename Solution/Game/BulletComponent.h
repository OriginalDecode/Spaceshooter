#pragma once
#include "Component.h"
#include "Enums.h"

class BulletComponent : public Component
{
public:
	BulletComponent(Entity& aEntity);
	
	void Update(float aDeltaTime) override;
	void Init(float aMaxTime, int aDamage);

	void ReceiveNote(const CollisionNote& aNote) override;

	static eComponentType GetType();

	void SetMaxLifeTime(float aMaxTime);
	void SetActive(bool aActive);
	bool GetActive() const;

private:
	eBulletType myType;
	float myCurrentLifeTime;
	float myMaxLifeTime;
	bool myActive;
	int myDamage;
	// damage, radius, etc
};

inline eComponentType BulletComponent::GetType()
{
	return eComponentType::BULLET;
}

inline void BulletComponent::SetMaxLifeTime(float aMaxTime)
{
	myMaxLifeTime = aMaxTime;
}

inline void BulletComponent::SetActive(bool aActive)
{
	myActive = aActive;
}

inline bool BulletComponent::GetActive() const
{
	return myActive;
}