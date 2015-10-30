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
	myInvulnerablityTimeCurrent = 0.f;
	myInvulnerablityTimeMax = 0.f;
	DL_ASSERT_EXP(myMaxHealth > 0, "Health component inited to 0 hp.");
}

void HealthComponent::AddHealth(int aHealthToAdd)
{
	myCurrentHealth += aHealthToAdd;

	if (myCurrentHealth > myMaxHealth)
	{
		myCurrentHealth = myMaxHealth;
	}
	
	myEntity.SendNote(HealthNote(myCurrentHealth, myMaxHealth));
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

void HealthComponent::Update(float aDeltaTime)
{
	if (myIsInvulnerable == true)
	{
		myInvulnerablityTimeCurrent += aDeltaTime;
		if (myInvulnerablityTimeCurrent >= myInvulnerablityTimeMax)
		{
			myInvulnerablityTimeCurrent = 0.f;
			myInvulnerablityTimeMax = 0.f;
			myIsInvulnerable = false;
		}
	}
}

void HealthComponent::ReceiveNote(const PowerUpNote& aNote)
{
	if (aNote.myType == ePowerUpType::HEALTHKIT)
	{
		AddHealth(static_cast<int>(aNote.myValue));
	}
	else if (aNote.myType == ePowerUpType::INVULNERABLITY)
	{
		if (myIsInvulnerable == true)
		{
			myInvulnerablityTimeCurrent = 0.f;
			myInvulnerablityTimeMax = aNote.myDuration;
		}
		else
		{
			myIsInvulnerable = true;
			myInvulnerablityTimeCurrent = 0.f;
			myInvulnerablityTimeMax = aNote.myDuration;
		}

	}
}

void HealthComponent::Reset()
{
	myIsInvulnerable = false;
	myCurrentHealth = myMaxHealth;
}