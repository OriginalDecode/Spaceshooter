#include "StateStack.h"
#include <DL_Assert.h>
#include "GameState.h"

StateStack::StateStack()
{
	myMainIndex = -1;
	mySubIndex = -1;
	myGameStates.Init(16);
}

StateStack::~StateStack()
{
}

void StateStack::PopSubGameState()
{
	DL_ASSERT_EXP(myGameStates.Size() > 0, "Can't pop an empty stack.");

	myGameStates[myMainIndex][mySubIndex]->EndState();

	myGameStates[myMainIndex].DeleteCyclicAtIndex(mySubIndex);
	--mySubIndex;

	if (mySubIndex >= 0)
	{
		myGameStates[myMainIndex][mySubIndex]->ResumeState();
	}
}

void StateStack::PopMainGameState()
{
	while (myGameStates[myMainIndex].Size() > 0)
	{
		PopSubGameState();
	}

	myGameStates.RemoveCyclicAtIndex(myMainIndex);
	--myMainIndex;

	if (myMainIndex >= 0)
	{
		mySubIndex = myGameStates[myMainIndex].Size() - 1;
	}
	else
	{
		mySubIndex = -1;
	}

	if (myMainIndex >= 0 && mySubIndex >= 0)
	{
		myGameStates[myMainIndex][mySubIndex]->ResumeState();
	}
}

void StateStack::PushSubGameState(GameState* aSubGameState)
{
	DL_ASSERT_EXP(myGameStates.Size() > 0, "Can't push sub game state, no main game state present.");
	DL_ASSERT_EXP(myMainIndex < 20 && mySubIndex < 20, "Can't add more than 20 states, it's unreasonable!");

	myGameStates[myMainIndex].Add(aSubGameState);
	aSubGameState->InitState();

	mySubIndex = myGameStates[myMainIndex].Size() - 1;
}

void StateStack::PushMainGameState(GameState* aMainGameState)
{
	myGameStates.Add(CU::GrowingArray<GameState*, int>(16));
	myMainIndex = myGameStates.Size() - 1;
	PushSubGameState(aMainGameState);
}

bool StateStack::UpdateCurrentState()
{
	
	switch (myGameStates[myMainIndex][mySubIndex]->Update())
	{
	case eStateStatus::ePopSubState:
		PopSubGameState();
		break;
	case eStateStatus::ePopMainState:
		PopMainGameState();
		break;
	case eStateStatus::eKeepState:
		break;
	}

	return myGameStates.Size() > 0;
}

void StateStack::RenderCurrentState()
{
	DL_ASSERT_EXP(myGameStates.Size() > 0, "Can't render, no gamestate present.");
	RenderStateAtIndex(mySubIndex);
}

void StateStack::RenderStateAtIndex(int aIndex)
{
	if (aIndex < 0)
	{
		return;
	}

	if (myGameStates[myMainIndex][aIndex]->IsLetThroughRender() == true)
	{
		RenderStateAtIndex(aIndex - 1);
	}

	myGameStates[myMainIndex][aIndex]->Render();
}

void StateStack::Clear()
{
	while (myGameStates.Size() > 0)
	{
		PopMainGameState();
	}
}