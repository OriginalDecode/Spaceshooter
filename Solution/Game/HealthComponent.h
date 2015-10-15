#pragma once
#include "Component.h"

class HealthComponent : public Component
{
public:
	HealthComponent(Entity& aEntity);

	void Init(int aMaxHealth, bool anIsInvulnerable = false);
	void AddHealth(int aHealthToAdd);
	void RemoveHealth(int aHealthToRemove);

	void ReceiveNote(const PowerUpNote& aNote) override;

	bool IsAlive() const;

	int GetHealth() const;
	
	void SetInvulnerability(bool anIsInvulnerable);
	bool GetInvulnerability() const;

	static eComponentType GetType();
private:

	int myMaxHealth;
	int myCurrentHealth;
	bool myIsInvulnerable;
};

inline bool HealthComponent::IsAlive() const
{
	return (myCurrentHealth != 0);
}

inline int HealthComponent::GetHealth() const
{
	return myCurrentHealth;
}

inline void HealthComponent::SetInvulnerability(bool anIsInvulnerable)
{
	myIsInvulnerable = anIsInvulnerable;
}

inline bool HealthComponent::GetInvulnerability() const
{
	return myIsInvulnerable;
}

inline eComponentType HealthComponent::GetType()
{
	return eComponentType::HEALTH;
}