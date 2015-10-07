#pragma once
#include <string>
class Entity;

class MissionManager
{
public:
	MissionManager(Entity& aPlayer, const std::string& aFileToReadFrom);

private:
	Entity& myPlayer;

};

