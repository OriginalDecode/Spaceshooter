#include "stdafx.h"

#include "HealthComponent.h"
#include "Entity.h"
#include "Enums.h"
#include "HealthNote.h"
#include "PowerUpNote.h"
#include "ShieldComponent.h"

HealthComponent::HealthComponent(Entity& aEntity)
	: Component(aEntity)
{
}

void HealthComponent::Init(int aMaxHealth, bool anIsInvulnerable)
{
	myMaxHealth = aMaxHealth;
	myCurrentHealth = myMaxHealth;
	myIsInvulnerable = anIsInvulnerable;
	DL_ASSERT_EXP(myMaxHealth > 0, "Health component inited to 0 hp.");
}

void HealthComponent::AddHealth(int aHealthToAdd)
{
	myCurrentHealth += aHealthToAdd;

	if (myIsInvulnerable == false) // testing
	{
		if (myCurrentHealth > myMaxHealth)
		{
			myCurrentHealth = myMaxHealth;
		}
	}
}

void HealthComponent::RemoveHealth(int aHealthToRemove)
{
	if (myIsInvulnerable == false)
	{
		if (myCurrentHealth <= aHealthToRemove)
		{
			myCurrentHealth = 0;
			myEntity.Kill();
		}
		else
		{
			myCurrentHealth -= aHealthToRemove;
		}
		
		myEntity.SendNote(HealthNote(myCurrentHealth, myMaxHealth));

	}
}

void HealthComponent::ReceiveNote(const PowerUpNote& aNote)
{
	if (aNote.myType == ePowerUpType::HEALTHKIT)
	{
		AddHealth(static_cast<int>(aNote.myValue));
	}
}