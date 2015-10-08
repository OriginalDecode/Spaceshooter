#pragma once
#include "Component.h"
#include "Enums.h"
class ShieldComponent : public Component
{
public:
	ShieldComponent(Entity& aEntity);


	void Init() override;
	void ReceiveNote(const PowerUpNote& aNote) override;

	void DamageShield(unsigned short someDamage);
	int GetCurrentShieldStrength();
	void Update(float aDelta) override;

	static int GetID();


private:

	bool myShieldOvercharged;

	float myReachargeTime;
	float myCooldown;


	int myShieldStrength;
	int myOvercharge;

};

inline int ShieldComponent::GetCurrentShieldStrength()
{
	return myShieldStrength;
}

inline int ShieldComponent::GetID()
{
	return 13;
}
