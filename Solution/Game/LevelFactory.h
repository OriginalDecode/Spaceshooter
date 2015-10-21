#pragma once

namespace CommonUtilities
{
	class InputWrapper;
}

namespace Prism
{
	class DirectionalLight;
	class PointLight;
	class SpotLight;
};

class Level;

class LevelFactory
{
public:
	LevelFactory(const std::string& aLevelListPath, CU::InputWrapper* anInputWrapper);
	~LevelFactory();

	Level* LoadLevel(const int& anID);
	Level* LoadCurrentLevel();
	Level* LoadNextLevel();
	
	bool IsLastLevel() const;
	void LoadLevelListFromXML(const std::string& aXMLPath); 
	void ReadXML(const std::string& aFilePath);

private:

	void LoadLights(XMLReader& aReader, tinyxml2::XMLElement* aLevelElement);
	void LoadDirectionalLights(XMLReader& aReader, tinyxml2::XMLElement* aLevelElement);
	void LoadProps(XMLReader& aReader, tinyxml2::XMLElement* aLevelElement);
	void LoadTriggers(XMLReader& aReader, tinyxml2::XMLElement* aLevelElement);
	void LoadPowerups(XMLReader& aReader, tinyxml2::XMLElement* aLevelElement);
	void LoadPlayer();

	void AddToScene();

	void SetSkySphere(const std::string& aModelFilePath, const std::string& aEffectFileName);

	CU::InputWrapper* myInputWrapper;
	Level* myCurrentLevel;
	Level* myOldLevel;

	CU::GrowingArray<Prism::DirectionalLight*> myDirectionalLights;
	CU::GrowingArray<Prism::PointLight*> myPointLights;
	CU::GrowingArray<Prism::SpotLight*> mySpotLights;

	std::unordered_map<int, std::string> myLevelPaths;

	int myCurrentID;
};

