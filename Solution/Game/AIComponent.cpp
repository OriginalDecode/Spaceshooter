#include "stdafx.h"
#include "AIComponent.h"

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
		MoveLeft(150.f * aDeltaTime);
		break;
	case 1:
		MoveRight(150.f * aDeltaTime);
		break;
	case 2:
		MoveUp(150.f * aDeltaTime);
		break;
	case 3:
		MoveDown(150.f * aDeltaTime);
		break;
	case 4:
		MoveForward(150.f * aDeltaTime);
		break;
	case 5:
		MoveBackward(150.f * aDeltaTime);
		break;
	}
}

void AIComponent::MakeDecision()
{
	myDecision = rand() % 6;
	myTimeToNextDecision = 5.f;
}
