#pragma once

#include "Enums.h"

class Message
{
public:
	Message(eMessageType aMessageType);
	eMessageType GetMessageType() const;

protected:
	
	const eMessageType myMessageType;
};

inline Message::Message(eMessageType aMessageType)
	:myMessageType(aMessageType)
{
}

inline eMessageType Message::GetMessageType() const
{
	return myMessageType;
}