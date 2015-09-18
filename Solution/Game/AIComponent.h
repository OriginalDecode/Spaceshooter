#pragma once
#include "ControllerComponent.h"

class AIComponent : public ControllerComponent
{
public:
	void Init();
	void Update(float aDeltaTime) override;

private:
	void MakeDecision();

	float myTimeToNextDecision;
	int myDecision;
};

