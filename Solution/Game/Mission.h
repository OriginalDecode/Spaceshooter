#pragma once
#include <GrowingArray.h>

class Event;

class Mission
{
public:
	Mission();
	virtual ~Mission();

	virtual bool Update(float aDeltaTime) = 0;
	virtual void Start();
	void EventsStart();
	virtual void End();
	void EventsEnd();

	virtual int GetIndex() const;
	virtual void SetIndex(int aIndex);

protected:
	int myIndex;
	CU::GrowingArray<Event*> myEventsStart;
	CU::GrowingArray<Event*> myEventsEnd;

};

