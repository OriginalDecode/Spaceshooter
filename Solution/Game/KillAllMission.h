#pragma once
#include "Mission.h"

class Level;

class KillAllMission : public Mission
{
public:
	KillAllMission(Level& aLevel);

	bool Update(float aDeltaTime) override;

private:
	Level& myLevel;
};

