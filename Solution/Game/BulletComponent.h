#pragma once
#include "Component.h"
#include "Enums.h"

class BulletComponent : public Component
{
public:
	BulletComponent(Entity& aEntity);
	
	void Update(float aDeltaTime) override;
	void Init(const float& aMaxTime, const unsigned short& aDamage);

	static int GetID();

	void SetMaxLifeTime(float aMaxTime);
	void SetActive(bool aActive);
	bool GetActive() const;
	unsigned short GetDamage() const;

private:
	eBulletType myType;
	float myCurrentLifeTime;
	float myMaxLifeTime;
	bool myActive;
	unsigned short myDamage;
	// damage, radius, etc
};

inline int BulletComponent::GetID()
{
	return 5;
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

inline unsigned short BulletComponent::GetDamage() const
{
	return myDamage;
}