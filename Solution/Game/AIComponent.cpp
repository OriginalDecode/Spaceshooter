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
		MoveLeft();
		break;
	case 1:
		MoveRight();
		break;
	case 2:
		MoveUp();
		break;
	case 3:
		MoveDown();
		break;
	case 4:
		MoveForward();
		break;
	case 5:
		MoveBackward();
		break;
	}
}

void AIComponent::MakeDecision()
{
	myDecision = rand() % 6;
	myTimeToNextDecision = 5.f;
}
