#pragma once

#include <Engine.h>
#include "Message.h"
#include <Vector.h>

class ResizeMessage : public Message
{
public:

	ResizeMessage();

	const CU::Vector2<int>& GetResolution() const;

private:
	
	CU::Vector2<int> myResolution;

};

inline ResizeMessage::ResizeMessage()
	: Message(eMessageType::RESIZE)
{
	myResolution = Prism::Engine::GetInstance()->GetWindowSize();
}

inline const CU::Vector2<int>& ResizeMessage::GetResolution() const
{
	return myResolution;
}
