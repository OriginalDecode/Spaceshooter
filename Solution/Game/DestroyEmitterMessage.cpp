#include "stdafx.h"
#include "EmitterMessage.h"


DestroyEmitterMessage::DestroyEmitterMessage(EmitterComponent* anEmitterComponent)
	: Message(eMessageType::DESTORY_EMITTER)
	, myEmitterComponent(anEmitterComponent)
{
}


DestroyEmitterMessage::~DestroyEmitterMessage()
{
}
