#pragma once
#include "Component.h"

class HealthComponent : public Component
{
public:
	HealthComponent(Entity& aEntity);

	void Init(const unsigned short& aMaxHealth, const bool& anIsInvulnerable = false);
	void AddHealth(const unsigned short& aHealthToAdd);
	void RemoveHealth(const unsigned short& aHealthToRemove);

	void ReceiveNote(const PowerUpNote& aNote) override;

	bool IsAlive() const;

	const unsigned short& GetHealth() const;
	
	void SetInvulnerability(const bool& anIsInvulnerable);
	const bool& GetInvulnerability() const;

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

inline void HealthComponent::SetInvulnerability(const bool& anIsInvulnerable)
{
	myIsInvulnerable = anIsInvulnerable;
}

inline const bool& HealthComponent::GetInvulnerability() const
{
	return myIsInvulnerable;
}