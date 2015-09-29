#pragma once
#include "Component.h"

class HealthComponent : public Component
{
public:
	HealthComponent();
	~HealthComponent();

	void Init(const unsigned short& aMaxHealth);
	void AddHealth(const unsigned short& aHealthToAdd);
	void RemoveHealth(const unsigned short& aHealthToRemove);

	const bool IsAlive() const;
	const unsigned short& GetHealth() const;

	static int GetID();

private:

	unsigned short myMaxHealth;
	unsigned short myCurrentHealth;
};

inline const bool HealthComponent::IsAlive() const
{
	return (myCurrentHealth != 0);
}

inline const unsigned short& HealthComponent::GetHealth() const
{
	return myCurrentHealth;
}

inline int HealthComponent::GetID()
{
	return 8;
}