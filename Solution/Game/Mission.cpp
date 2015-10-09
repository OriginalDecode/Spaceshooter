#include "stdafx.h"
#include "Mission.h"


Mission::Mission()
	: myIndex(-1)
{
}


Mission::~Mission()
{
}

void Mission::Start()
{

}

void Mission::End()
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
