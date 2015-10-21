#pragma once
#include "Message.h"

class ResizeMessage : public Message
{
public:

	ResizeMessage();


private:
	
	

};

inline ResizeMessage::ResizeMessage()
	: Message(eMessageType::RESIZE)
{



}