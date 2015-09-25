#pragma once
#include "StateEnums.h"

namespace CommonUtilities
{
	class InputWrapper;
}

class GameState
{
public:

	virtual void InitState(CU::InputWrapper* anInputWrapper) = 0;
	virtual void EndState() = 0;
	
	virtual const eStateStatus Update() = 0;
	virtual void Render() = 0;
	virtual void ResumeState() = 0;
	virtual void OnResize(int aWidth, int aHeight) = 0;

	const bool& IsLetThroughRender() const;

protected:

	CU::InputWrapper* myInputWrapper;
	eStateStatus myStateStatus;
	bool myIsLetThrough;
};

inline const bool& GameState::IsLetThroughRender() const
{
	return myIsLetThrough;
}