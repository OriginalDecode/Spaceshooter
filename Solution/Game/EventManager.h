#pragma once
#include <string>
#include "Subscriber.h"
#include <unordered_map>

class Event;
class ConversationManager;

class EventManager : public Subscriber
{
public:
	EventManager(const std::string& aXmlPath, ConversationManager& aConversationManager);
	~EventManager();

	void ReceiveMessage(const EnqueueEventMessage& aMessage) override;
	
	void Update();
private:
	std::unordered_map<std::string, Event*> myEvents;
	CU::GrowingArray<Event*> myEventQueue;
};

