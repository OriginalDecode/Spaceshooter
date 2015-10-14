#pragma once
#include <GrowingArray.h>

class Action;

class Event
{
public:
	Event(const CU::GrowingArray<Action*>& someActions);
	~Event();

	void Start();

private:
	CU::GrowingArray<Action*> myActions;
};

