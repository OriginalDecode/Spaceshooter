#include "stdafx.h"
#include "DestroyEmitterMessage.h"

DestroyEmitterMessage::DestroyEmitterMessage(EmitterComponent* anEmitterComponent)
	: Message(eMessageType::DESTORY_EMITTER)
	, myEmitterComponent(anEmitterComponent)
{
}


DestroyEmitterMessage::~DestroyEmitterMessage()
{
}
