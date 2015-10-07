#include "stdafx.h"
#include "LevelFactory.h"
#include "Level.h"

LevelFactory::LevelFactory()
{
}

LevelFactory::~LevelFactory()
{
}

Level* LevelFactory::CreateLevel(const int& ID)
{



	return myCurrentLevel;
}