#pragma once
#include <GrowingArray.h>

class Action;

class Event
{
public:
	Event();
	~Event();

	void Start();

private:
	CU::GrowingArray<Action*> myActions;
};

