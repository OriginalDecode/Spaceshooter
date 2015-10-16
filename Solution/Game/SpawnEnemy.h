#pragma once
#include "Action.h"

namespace tinyxml2
{
	class XMLElement;
}
class SpawnEnemyMessage;
class XMLReader;

class SpawnEnemy : public Action
{
public:
	SpawnEnemy(XMLReader& aReader, tinyxml2::XMLElement* aElement);
	~SpawnEnemy();

	void OnEnter() override;
	bool OnExit() override;
	bool Update() override;

private:
	SpawnEnemyMessage* myMessage;
};

