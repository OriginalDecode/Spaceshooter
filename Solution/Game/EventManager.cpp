#include "stdafx.h"
#include "CommonHelper.h"
#include "ConversationAction.h"
#include "Event.h"
#include "EventManager.h"
#include "PostMaster.h"
#include "SpawnEnemy.h"
#include "StartEventMessage.h"
#include <XMLReader.h>

EventManager::EventManager(const std::string& aXmlPath, ConversationManager& aConversationManager)
	: myActiveEvents(16)
{
	PostMaster::GetInstance()->Subscribe(eMessageType::START_EVENT, this);
	XMLReader reader;
	reader.OpenDocument(aXmlPath);
	tinyxml2::XMLElement* eventElement = reader.ForceFindFirstChild("root");
	
	for (eventElement = reader.FindFirstChild(eventElement, "event"); eventElement != nullptr;
		eventElement = reader.FindNextElement(eventElement, "event"))
	{
		std::string name;
		reader.ForceReadAttribute(eventElement, "name", name);
		name = CU::ToLower(name);

		CU::GrowingArray<Action*> actions(8);

		for (tinyxml2::XMLElement* element = reader.FindFirstChild(eventElement, "spawnEnemy"); element != nullptr;
			element = reader.FindNextElement(element, "spawnEnemy"))
		{
			// move to spawnEnemy-class, then read pos, rot, scale.
			actions.Add(new SpawnEnemy(reader, element));

		}
		tinyxml2::XMLElement* element = reader.FindFirstChild(eventElement, "conversation");
		if (element != nullptr)
		{
			actions.Add(new ConversationAction(name, aConversationManager));
		}
		myEvents[name] = new Event(name, actions);
	}
	reader.CloseDocument();
}


EventManager::~EventManager()
{
	PostMaster::GetInstance()->UnSubscribe(eMessageType::START_EVENT, this);
	for (auto it = myEvents.begin(); it != myEvents.end(); ++it)
	{
		delete it->second;
	}
}

void EventManager::ReceiveMessage(const StartEventMessage& aMessage)
{
	std::string eventName = CU::ToLower(aMessage.GetName());
	std::unordered_map<std::string, Event*>::const_iterator it = myEvents.find(eventName);

	if (it == myEvents.end())
	{
		DL_ASSERT(("Event does not exist: " + eventName).c_str());
	}
	else
	{
		myActiveEvents.Add(myEvents[eventName]);
		if (myActiveEvents.Size() == 1)
		{
			myActiveEvents.GetLast()->Start();
		}
	}
}

void EventManager::Update()
{
	if (myActiveEvents.Size() > 0 && myActiveEvents[0]->Update() == true)
	{
		myActiveEvents.RemoveNonCyclicAtIndex(0);
		
		if (myActiveEvents.Size() > 0)
		{
			myActiveEvents[0]->Start();
		}
	}
}