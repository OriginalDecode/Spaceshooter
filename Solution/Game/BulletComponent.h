#pragma once
#include "Component.h"
#include "BulletEnums.h"

class BulletComponent : public Component
{
public:
	BulletComponent();
	~BulletComponent();
	
	void Update(float aDeltaTime) override;
	void Init(const float& aMaxTime, const unsigned short& aDamage);

	static int GetID();

	void SetMaxLifeTime(const float& aMaxTime);
	void SetIsActive(const bool& aIsActive);
	const bool& GetIActive();
	const unsigned short& GetDamage() const;

private:
	eBulletType myType;
	float myCurrentLifeTime;
	float myMaxLifeTime;
	bool myIsActive;
	unsigned short myDamage;
	// damage, radius, etc
};

inline int BulletComponent::GetID()
{
	return 5;
}

inline void BulletComponent::SetMaxLifeTime(const float& aMaxTime)
{
	myMaxLifeTime = aMaxTime;
}

inline void BulletComponent::SetIsActive(const bool& aIsActive)
{
	myIsActive = aIsActive;
}

inline const bool& BulletComponent::GetIActive()
{
	return myIsActive;
}

inline const unsigned short& BulletComponent::GetDamage() const
{
	return myDamage;
}