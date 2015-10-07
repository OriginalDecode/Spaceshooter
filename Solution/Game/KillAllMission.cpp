#include "stdafx.h"
#include "Engine.h"
#include "KillAllMission.h"
#include "Level.h"


KillAllMission::KillAllMission(Level& aLevel)
	: myLevel(aLevel)
{
}

bool KillAllMission::Update(float aDeltaTime)
{
	Prism::Engine::GetInstance()->PrintDebugText("KILL ALL!", { 400, -400 });

	return myLevel.GetEnemiesAlive() == 0;
}
