#include "stdafx.h"
#include "Event.h"
#include "EventManager.h"
#include "SpawnEnemy.h"
#include <XMLReader.h>

EventManager::EventManager(const std::string& aXmlPath)
{
	XMLReader reader;
	reader.OpenDocument(aXmlPath);
	tinyxml2::XMLElement* element = reader.ForceFindFirstChild("root");
	
	for (element = reader.FindFirstChild(element, "event"); element != nullptr;
		element = reader.FindNextElement(element, "event"))
	{
		std::string name;
		reader.ForceReadAttribute(element, "name", name);

		CU::GrowingArray<Action*> actions;

		for (element = reader.FindFirstChild(element, "spawnEnemy"); element != nullptr;
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
