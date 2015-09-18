#pragma once

class Message;
class BulletMessage;

class Subscriber
{
public:
	Subscriber();
	~Subscriber();

	virtual void ReceiveMessage(const Message& aMessage);
	virtual void ReceiveMessage(const BulletMessage& aMessage);
};

