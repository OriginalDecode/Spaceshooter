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
	Level* ReloadLevel();

private:

	CU::InputWrapper* myInputWrapper;
	Level* myCurrentLevel;

	std::unordered_map<int, std::string> myLevelPaths;

	int myCurrentID;
};

