#include "stdafx.h"

#include "Enums.h"
#include "PowerUpNote.h"
#include "ShieldComponent.h"


ShieldComponent::ShieldComponent(Entity& aEntity)
	: Component(aEntity)
{
}

void ShieldComponent::Init()
{

	myShieldOvercharged = false;
	
	myReachargeTime = 5.f;
	myCooldown = 0.f;

	myOvercharge = 0;
	myShieldStrength = 100;

}

void ShieldComponent::ReceiveNote(const PowerUpNote& aNote)
{
	if (aNote.myType == ePowerUpType::SHIELDBOOST)
	{
		myShieldOvercharged = true;
		myShieldStrength = aNote.myShieldStrength;
	}
}

void ShieldComponent::DamageShield(int someDamage)
{
	myCooldown = 0;
	if (someDamage >= myShieldStrength)
	{
		myShieldStrength = 0;
	}
	else
	{
		myShieldStrength -= someDamage;
	}
}

void ShieldComponent::Update(float aDelta)
{
	if (myShieldOvercharged == false)
	{
		if (myShieldStrength <= 0)
		{
			COMPONENT_LOG("Shield is depleted, setting to 0");
			myShieldStrength = 0;
		}

		if (myShieldStrength < 100)
		{
			COMPONENT_LOG("Recharging shield strenght!");
			myCooldown += aDelta;

			if (myCooldown >= myReachargeTime)
			{
				if (myShieldStrength >= 100)
				{
					COMPONENT_LOG("Shield Strength reached 100. During recharge time.");
					myShieldStrength = 100;
					return;
				}
				myShieldStrength += 1;
			}
		}
	}

	if (myShieldOvercharged == true)
	{
		COMPONENT_LOG("Shield is overcharged.");
		POWERUP_LOG("Shield is overcharged.");
		if (myShieldStrength <= 100)
		{
			COMPONENT_LOG("Shield is 100 or lower, shield is no longer overcharged!");
			myShieldOvercharged = false;
		}
	}

}

