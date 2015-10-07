#pragma once
#include "Component.h"

class HealthComponent : public Component
{
public:
	HealthComponent(Entity& aEntity);

	void Init(const unsigned short& aMaxHealth, const bool& anIsInvulnerable = false);
	void AddHealth(const unsigned short& aHealthToAdd);
	void RemoveHealth(const unsigned short& aHealthToRemove);

	bool IsAlive() const;

	const unsigned short& GetHealth() const;

private:

	unsigned short myMaxHealth;
	unsigned short myCurrentHealth;
	bool myIsInvulnerable;
};

inline bool HealthComponent::IsAlive() const
{
	return (myCurrentHealth != 0);
}

inline const unsigned short& HealthComponent::GetHealth() const
{
	return myCurrentHealth;
}