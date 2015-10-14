#include "stdafx.h"
#include "StartEventMessage.h"


StartEventMessage::StartEventMessage(const std::string& aName)
	: myName(aName)
	, Message(eMessageType::START_EVENT)
{
}

const std::string& StartEventMessage::GetName() const
{
	return myName;
}
