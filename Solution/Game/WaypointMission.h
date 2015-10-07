#pragma once

#include "Mission.h"
class Entity;
class Level;
class XMLReader;
namespace tinyxml2
{
	class XMLElement;
}

class WaypointMission :	public Mission
{
public:
	WaypointMission(Level& aLevel, XMLReader& aReader, tinyxml2::XMLElement* aElement);
	~WaypointMission();

	bool Update(float aDeltaTime) override;

private:
	Level& myLevel;
	Entity* myTrigger;
};

