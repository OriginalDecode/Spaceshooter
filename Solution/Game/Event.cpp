#include "stdafx.h"
#include "Action.h"
#include "Event.h"

Event::Event(const std::string& aName, const CU::GrowingArray<Action*>& someActions)
	: myName(aName)
	, myActions(someActions)
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
