#pragma once
#include "StateEnums.h"

class GameState
{
public:

	virtual void InitState() = 0;
	virtual void EndState() = 0;
	
	virtual const eStateStatus& Update() = 0;
	virtual void Render() = 0;
	virtual void ResumeState() = 0;

	const bool& IsLetThroughRender() const;

protected:

	eStateStatus myStateStatus;
	bool myIsLetThrough;
};

inline const bool& GameState::IsLetThroughRender() const
{
	return myIsLetThrough;
}