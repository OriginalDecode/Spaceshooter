#pragma once
#include "Component.h"
#include "Enums.h"
class ShieldComponent : public Component
{
public:
	ShieldComponent(Entity& aEntity);


	void Init() override;
	void ReceiveNote(const PowerUpNote& aNote) override;

	void Update(float aDelta) override;

private:

	bool myShieldOvercharged;

	float myReachargeTime;
	float myCooldown;


	int myShieldStrength;
	int myOvercharge;

};

