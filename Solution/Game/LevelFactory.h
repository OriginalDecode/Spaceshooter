#pragma once

class Level;

class LevelFactory
{
public:
	LevelFactory();
	~LevelFactory();

	Level* CreateLevel(const int& ID);

private:

	Level* myCurrentLevel;

	CU::GrowingArray<std::string> myLevelPaths;
};

