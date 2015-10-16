#pragma once
#include <GrowingArray.h>
#include <string>

class Action;

class Event
{
public:
	Event(const std::string& aName, const CU::GrowingArray<Action*>& someActions);
	~Event();

	void Start();

private:
	void operator=(Event&) = delete;
	const std::string myName;
	CU::GrowingArray<Action*> myActions;
};

