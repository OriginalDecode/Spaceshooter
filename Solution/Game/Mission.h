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
	virtual void End();

	virtual int GetIndex() const;
	virtual void SetIndex(int aIndex);

protected:
	int myIndex;
	CU::GrowingArray<Event*> myStartEvents;

};

