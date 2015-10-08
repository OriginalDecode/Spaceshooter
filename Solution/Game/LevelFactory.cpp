#include "stdafx.h"
#include <FileWatcher.h>
#include "LevelFactory.h"
#include "Level.h"
#include "PostMaster.h"
#include "GameStateMessage.h"
#include <XMLReader.h>

LevelFactory::LevelFactory(const std::string& aLevelListPath, CU::InputWrapper* anInputWrapper, const bool& aCanWinGame)
	: myInputWrapper(anInputWrapper)
	, myCurrentLevel(nullptr)
	, myOldLevel(nullptr)
	, myLevelPaths(8)
	, myCurrentID(0)
	, myCanWinGame(aCanWinGame)
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
	myCurrentLevel = nullptr;
	UNWATCH_FILE(myLevelPaths[myCurrentID]);
	myCurrentID = anID;
	WATCH_FILE(myLevelPaths[myCurrentID], LevelFactory::LoadLevelFromXML);
	
	return ReloadLevel();
}

Level* LevelFactory::ReloadLevel()
{
	myOldLevel = myCurrentLevel;
	myCurrentLevel = new Level(myLevelPaths[myCurrentID], myInputWrapper);
	return myCurrentLevel;
}

Level* LevelFactory::LoadNextLevel()
{
	if (myLevelPaths.find(myCurrentID + 1) == myLevelPaths.end())
	{
		if (myCanWinGame == true)
		{
			return myCurrentLevel;
		}
		else
		{
			myCurrentID = 0;
		}
	}

	UNWATCH_FILE(myLevelPaths[myCurrentID]);
	myCurrentID++;
	WATCH_FILE(myLevelPaths[myCurrentID], LevelFactory::LoadLevelFromXML);

	return ReloadLevel();
}

void LevelFactory::DeleteOldLevel()
{
	delete myOldLevel;
	myOldLevel = nullptr;
}

const bool& LevelFactory::IsLastLevel() const
{
	if (myCanWinGame == true && myLevelPaths.find(myCurrentID + 1) == myLevelPaths.end())
	{
		return true;
	}
	return false;
}

void LevelFactory::LoadLevelListFromXML(const std::string& aXMLPath)
{
	myLevelPaths.clear();
	XMLReader reader;
	reader.OpenDocument(aXMLPath);
	std::string levelPath = "";
	int ID = 0;
	int lastID = ID - 1;

	tinyxml2::XMLElement* levelElement = reader.FindFirstChild("level");
	for (; levelElement != nullptr; levelElement = reader.FindNextElement(levelElement))
	{
		lastID = ID;
		
		reader.ForceReadAttribute(levelElement, "ID", ID);
		reader.ForceReadAttribute(levelElement, "path", levelPath);
		myLevelPaths[ID] = levelPath;

		if (ID - 1 != lastID)
		{
			DL_ASSERT("Wrong ID-number in levelList.xml! The numbers should be counting up, in order.");
		}
	}
}

void LevelFactory::LoadLevelFromXML(const std::string&)
{
	// this just reloads the current level 
	PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::RELOAD_LEVEL));
}