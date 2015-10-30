#pragma once
#include "Message.h"

class EmitterComponent;

class DestroyEmitterMessage : public Message
{
public:
	DestroyEmitterMessage(EmitterComponent* anEmitterComponent);
	~DestroyEmitterMessage();
	
	const EmitterComponent* myEmitterComponent;

};

