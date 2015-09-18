#pragma once

#include "Enums.h"

class Message
{
public:
	inline const eMessageType GetMessageType() const;

protected:
	
	eMessageType myMessageType;
};

inline const eMessageType Message::GetMessageType() const
{
	return myMessageType;
}