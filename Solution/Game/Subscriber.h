#pragma once

class TempMessage;

class Subscriber
{
public:
	Subscriber();
	~Subscriber();

	virtual void ReceiveMessage(const TempMessage& aMessage);
};

