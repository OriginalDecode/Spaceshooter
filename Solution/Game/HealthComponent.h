#pragma once
#include "Component.h"

class HealthComponent : public Component
{
public:
	HealthComponent(Entity& aEntity);

	void Init(unsigned short aMaxHealth, bool anIsInvulnerable = false);
	void AddHealth(unsigned short aHealthToAdd);
	void RemoveHealth(unsigned short aHealthToRemove);

	void ReceiveNote(const PowerUpNote& aNote) override;

	bool IsAlive() const;

	unsigned short GetHealth() const;
	
	void SetInvulnerability(bool anIsInvulnerable);
	bool GetInvulnerability() const;

	static eComponentType GetType();
private:

	unsigned short myMaxHealth;
	unsigned short myCurrentHealth;
	bool myIsInvulnerable;
};

inline bool HealthComponent::IsAlive() const
{
	return (myCurrentHealth != 0);
}

inline unsigned short HealthComponent::GetHealth() const
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