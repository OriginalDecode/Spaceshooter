#pragma once

namespace CommonUtilities
{
	class InputWrapper;
}

class Level;

class LevelFactory
{
public:
	LevelFactory(const std::string& aLevelListPath, CU::InputWrapper* anInputWrapper);
	~LevelFactory();

	Level* LoadLevel(const int& anID);
	Level* LoadCurrentLevel();
	Level* LoadNextLevel();

	void ReadXML(const std::string& aFilePath);

	bool IsLastLevel() const;

	void LoadLevelListFromXML(const std::string& aXMLPath); 
private:

	CU::InputWrapper* myInputWrapper;
	Level* myCurrentLevel;
	Level* myOldLevel;

	std::unordered_map<int, std::string> myLevelPaths;

	int myCurrentID;
};

