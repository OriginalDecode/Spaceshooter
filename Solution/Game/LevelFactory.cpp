#include "stdafx.h"
#include "LevelFactory.h"
#include "Level.h"
#include <XMLReader.h>

LevelFactory::LevelFactory(const std::string& aLevelListPath, CU::InputWrapper* anInputWrapper)
	: myInputWrapper(anInputWrapper)
	, myCurrentLevel(nullptr)
	, myLevelPaths(8)
	, myCurrentID(0)
{
	XMLReader reader;
	reader.OpenDocument(aLevelListPath);
	std::string levelPath = "";
	int ID = -1;

	tinyxml2::XMLElement* levelElement = reader.FindFirstChild("level");
	for (; levelElement != nullptr; levelElement = reader.FindNextElement(levelElement))
	{
		reader.ForceReadAttribute(levelElement, "ID", ID);
		reader.ForceReadAttribute(levelElement, "path", levelPath);
		myLevelPaths[ID] = levelPath;
	}
}

LevelFactory::~LevelFactory()
{
}

Level* LevelFactory::LoadLevel(const int& anID)
{
	if (myLevelPaths.find(anID) == myLevelPaths.end())
	{
		DL_ASSERT("Non-existing ID in LoadLevel! ID must correspond with levelList.xml");
	}

	// this makes the program crash..?
	//if (myCurrentLevel != nullptr)
	//{
	//	delete myCurrentLevel;
	//	myCurrentLevel = nullptr;
	//}

	myCurrentID = anID;
	
	return ReloadLevel();
}

Level* LevelFactory::ReloadLevel()
{
	myCurrentLevel = new Level(myLevelPaths[myCurrentID], myInputWrapper);

	return myCurrentLevel;
}

Level* LevelFactory::LoadNextLevel()
{
	if (myLevelPaths.find(myCurrentID + 1) == myLevelPaths.end())
	{
		return myCurrentLevel;
	}

	myCurrentID++;

	return ReloadLevel();
}