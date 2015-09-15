#include "stdafx.h"
#include "Debug_BlockTimer.h"


Debug_BlockTimer::Debug_BlockTimer(const char* aName)
	: myName(aName)
{
	Engine::GetInstance()->GetDebugDisplay().StartFunctionTimer(myName);
}


Debug_BlockTimer::~Debug_BlockTimer()
{
	Engine::GetInstance()->GetDebugDisplay().EndFunctionTimer(myName);
}
