#pragma once
#include "Mission.h"

class Entity;
class XMLReader;
namespace tinyxml2
{
	class XMLElement;
}

class DefendMission : public Mission
{
public:
	DefendMission(XMLReader& aReader, tinyxml2::XMLElement* aElement);
	~DefendMission();
	
	void Start() override;
	bool Update(float aDeltaTime) override;

	void End() override;

private:
	bool operator=(DefendMission&) = delete;
	std::string myNameToDefend;

	float myTime;
	float myTotalTime;
};

