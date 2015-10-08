#pragma once

namespace CommonUtilities
{
	class InputWrapper;
}

class Level;

class LevelFactory
{
public:
	LevelFactory(const std::string& aLevelListPath, CU::InputWrapper* anInputWrapper, const bool& aCanWinGame = true);
	~LevelFactory();

	Level* LoadLevel(const int& anID);
	Level* ReloadLevel();
	Level* LoadNextLevel();

	// for filewatcher:
	void LoadLevelListFromXML(const std::string& aXMLPath); 
	void LoadLevelFromXML(const std::string& aXMLPath);
private:

	CU::InputWrapper* myInputWrapper;
	Level* myCurrentLevel;

	std::unordered_map<int, std::string> myLevelPaths;

	int myCurrentID;
	bool myCanWinGame;
};

