#pragma once

#include "Mission.h"
class Entity;
class Level;
class XMLReader;
namespace tinyxml2
{
	class XMLElement;
}

class WaypointAbortMission : public Mission
{
public:
	WaypointAbortMission(Level& aLevel, Entity& aPlayer, XMLReader& aReader, tinyxml2::XMLElement* aElement);
	~WaypointAbortMission();

	bool Update(float aDeltaTime) override;
	void Start() override;
	void End() override;

private:
	bool operator=(WaypointAbortMission&) = delete;


	Level& myLevel;
	Entity* myTrigger;
	Entity& myPlayer;
};

