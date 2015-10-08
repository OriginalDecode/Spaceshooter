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
	//if (myCurrentLevel != nullptr)
	//{
	//	delete myCurrentLevel;
	//}

	UNWATCH_FILE(myLevelPaths[myCurrentID]);
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
		if (myCanWinGame == true)
		{
			PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::COMPLETE_GAME));
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

void LevelFactory::LoadLevelFromXML(const std::string&)
{
	// this just reloads the current level 
	PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::RELOAD_LEVEL));
	//myCurrentLevel = new Level(aXMLPath, myInputWrapper);
}