#include "stdafx.h"
#include "AIComponent.h"
#include "Constants.h"

void AIComponent::Init()
{

}

void AIComponent::Update(float aDeltaTime)
{
	myTimeToNextDecision -= aDeltaTime;
	if (myTimeToNextDecision <= 0.f)
	{
		MakeDecision();
	}

	switch (myDecision)
	{
	case 0:
		MoveLeft(10.f * aDeltaTime);
		break;
	case 1:
		MoveRight(10.f * aDeltaTime);
		break;
	case 2:
		MoveUp(10.f * aDeltaTime);
		break;
	case 3:
		MoveDown(10.f * aDeltaTime);
		break;
	case 4:
		MoveForward(10.f * aDeltaTime);
		break;
	case 5:
		MoveBackward(10.f * aDeltaTime);
		break;
	case 6:
		RotateX(globalPi / 4.f * aDeltaTime);
		break;
	case 7:
		RotateY(globalPi / 4.f * aDeltaTime);
		break;
	case 8:
		RotateZ(globalPi / 4.f * aDeltaTime);
		break;
	}
}

void AIComponent::MakeDecision()
{
	myDecision = rand() % 9;
	myTimeToNextDecision = 1.f;
}
