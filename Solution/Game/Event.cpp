#include "stdafx.h"
#include "Action.h"
#include "Event.h"

Event::Event(const CU::GrowingArray<Action*>& someActions)
	: myActions(someActions)
{
}


Event::~Event()
{
	myActions.DeleteAll();
}

void Event::Start()
{
	for (int i = 0; i < myActions.Size(); ++i)
	{
		myActions[i]->Start();
	}
}
