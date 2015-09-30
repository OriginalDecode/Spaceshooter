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

	bool IsAlive() const;

private:

	unsigned short myMaxHealth;
	unsigned short myCurrentHealth;
};

inline bool HealthComponent::IsAlive() const
{
	return (myCurrentHealth != 0);
}