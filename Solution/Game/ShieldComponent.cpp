#include "stdafx.h"
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
	
}

void ShieldComponent::Update(float aDelta)
{
	if (myShieldOvercharged == false)
	{
		if (myShieldStrength <= 0)
		{
			myShieldStrength = 0;
		}

		if (myShieldStrength < 100)
		{
			myCooldown += aDelta;

			if (myCooldown >= myReachargeTime)
			{
				myShieldStrength += 1;
			}

		}
	}
}

