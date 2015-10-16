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
		myActions[i]->OnEnter();
	}
}

bool Event::Update()
{
	if (myActions[myIndex]->Update() == true)
	{
		if (myActions[myIndex]->OnExit() == false)
		{
			myIndex = 0;
			return true;
		}

		++myIndex;

		if (myIndex < myActions.Size())
		{
			myActions[myIndex]->OnEnter();
		}
		else
		{
			myIndex = 0;
			return true;
		}
	}
	return false;
}