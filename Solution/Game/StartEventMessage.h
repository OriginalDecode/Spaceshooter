#pragma once

#include "Message.h"

class StartEventMessage : public Message
{
public:
	StartEventMessage(const std::string& aName);

	const std::string& GetName() const;

private:
	const std::string myName;
};

