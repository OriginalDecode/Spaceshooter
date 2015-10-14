#include "stdafx.h"
#include "Mission.h"
#include "Event.h"
#include "EventFactory.h"

Mission::Mission(XMLReader& aReader, tinyxml2::XMLElement* aElement)
	: myIndex(-1)
	, myStartEventNames(4)
{
	EventFactory::ReadEventNames(aReader, aElement, myStartEventNames);
}


Mission::~Mission()
{
}

void Mission::Start()
{
}

void Mission::EventsStart()
{
	for (int i = 0; i < myStartEventNames.Size(); ++i)
	{
		//myEventsStart[i]->Start();
	}
}

void Mission::End()
{

}

void Mission::EventsEnd()
{

}

int Mission::GetIndex() const
{
	return myIndex;
}

void Mission::SetIndex(int aIndex)
{
	myIndex = aIndex;
}
