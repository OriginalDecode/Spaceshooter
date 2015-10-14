#pragma once
#include <string>
#include "Subscriber.h"
#include <unordered_map>

class Event;

class EventManager : public Subscriber
{
public:
	EventManager(const std::string& aXmlPath);
	~EventManager();

	void ReceiveMessage(const SpawnEnemyMessage& aMessage) override;

private:
	std::unordered_map<std::string, Event*> myEvents;
};

