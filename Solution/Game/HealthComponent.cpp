#include "stdafx.h"
#include "HealthComponent.h"

HealthComponent::HealthComponent()
{
}

HealthComponent::~HealthComponent()
{
}

void HealthComponent::Init(const unsigned short& aMaxHealth)
{
	myMaxHealth = aMaxHealth;
	myCurrentHealth = myMaxHealth;
}

void HealthComponent::AddHealth(const unsigned short& aHealthToAdd)
{
	myCurrentHealth += aHealthToAdd;

	if (myCurrentHealth > myMaxHealth)
	{
		myCurrentHealth = myMaxHealth;
	}
}

void HealthComponent::RemoveHealth(const unsigned short& aHealthToRemove)
{
	if (myCurrentHealth < aHealthToRemove)
	{
		myCurrentHealth = 0;
	}
	else
	{
		myCurrentHealth -= aHealthToRemove;
	}
}