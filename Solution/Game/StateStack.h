#pragma once
#include <GrowingArray.h>

class GameState;

class StateStack
{
public:
	StateStack();
	~StateStack();

	void PushSubGameState(GameState* aSubGameState);
	void PushMainGameState(GameState* aMainGameState);
	bool UpdateCurrentState();
	void RenderCurrentState();
	void Clear();

private:
	CU::GrowingArray<CU::GrowingArray<GameState*, int>, int> myGameStates;
	void PopSubGameState();
	void PopMainGameState();
	void RenderStateAtIndex(int aIndex);

	int myMainIndex;
	int mySubIndex;
};

