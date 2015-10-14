#include "stdafx.h"
#include "Event.h"
#include "EventManager.h"
#include "SpawnEnemy.h"
#include <XMLReader.h>

EventManager::EventManager(const std::string& aXmlPath)
{
	XMLReader reader;
	reader.OpenDocument(aXmlPath);
	tinyxml2::XMLElement* eventElement = reader.ForceFindFirstChild("root");
	
	for (eventElement = reader.FindFirstChild(eventElement, "event"); eventElement != nullptr;
		eventElement = reader.FindNextElement(eventElement, "event"))
	{
		std::string name;
		reader.ForceReadAttribute(eventElement, "name", name);

		CU::GrowingArray<Action*> actions(8);

		for (tinyxml2::XMLElement* element = reader.FindFirstChild(eventElement, "spawnEnemy"); element != nullptr;
			element = reader.FindNextElement(element, "spawnEnemy"))
		{
			// move to spawnEnemy-class, then read pos, rot, scale.
			actions.Add(new SpawnEnemy(reader, element));

		}
		myEvents[name] = new Event(actions);
	}
	reader.CloseDocument();
}


EventManager::~EventManager()
{
	for (auto it = myEvents.begin(); it != myEvents.end(); ++it)
	{
		delete it->second;
	}
}

void EventManager::ReceiveMessage(const SpawnEnemyMessage& aMessage)
{
	int apa = 5;
	apa;
}
