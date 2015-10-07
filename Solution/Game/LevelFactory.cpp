#include "stdafx.h"
#include "LevelFactory.h"
#include "Level.h"
#include <XMLReader.h>
#include <FileWatcher.h>

LevelFactory::LevelFactory(const std::string& aLevelListPath, CU::InputWrapper* anInputWrapper)
	: myInputWrapper(anInputWrapper)
	, myCurrentLevel(nullptr)
	, myLevelPaths(8)
	, myCurrentID(0)
{
	LoadLevelListFromXML(aLevelListPath);

	WATCH_FILE(aLevelListPath, LevelFactory::LoadLevelListFromXML);
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

	//if (myCurrentLevel != nullptr)
	//{
	//	delete myCurrentLevel;
	//	myCurrentLevel = nullptr;
	//}

	myCurrentID = anID;

	WATCH_FILE(myLevelPaths[myCurrentID], LevelFactory::LoadLevelFromXML);
	
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

void LevelFactory::LoadLevelListFromXML(const std::string& aXMLPath)
{
	myLevelPaths.clear();

	XMLReader reader;
	reader.OpenDocument(aXMLPath);
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

void LevelFactory::LoadLevelFromXML(const std::string& aXMLPath)
{
	myCurrentLevel = new Level(aXMLPath, myInputWrapper);
}