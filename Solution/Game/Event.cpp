#include "stdafx.h"
#include "Action.h"
#include "Event.h"

Event::Event()
{
}


Event::~Event()
{
}

void Event::Start()
{
	for (int i = 0; i < myActions.Size(); ++i)
	{
		myActions[i]->Start();
	}
}
