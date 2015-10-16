#include "stdafx.h"
#include "Mission.h"
#include "Event.h"
#include "MissionEventReader.h"
#include "PostMaster.h"
#include "StartEventMessage.h"

Mission::Mission(XMLReader& aReader, tinyxml2::XMLElement* aElement)
	: myIndex(-1)
	, myStartEventNames(4)
{
	MissionEventReader::ReadEventNames(aReader, aElement, myStartEventNames);
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
		PostMaster::GetInstance()->SendMessage(StartEventMessage(myStartEventNames[i]));
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
