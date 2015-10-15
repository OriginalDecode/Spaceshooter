#pragma once
#include "Mission.h"

class Entity;
class Level;
class XMLReader;
namespace tinyxml2
{
	class XMLElement;
}

class KillXEnemiesMission : public Mission
{
public:
	KillXEnemiesMission(Level& aLevel, XMLReader& aReader, tinyxml2::XMLElement* aElement);

	bool Update(float aDeltaTime) override;

private:
	bool operator=(KillXEnemiesMission&) = delete;
	Level& myLevel;
	int myEnemiesToKill;
};

