#include "stdafx.h"
#include "PostMaster.h"
#include "SpawnEnemyAction.h"
#include "SpawnEnemyMessage.h"
#include <XMLReader.h>


SpawnEnemyAction::SpawnEnemyAction(XMLReader& aReader, tinyxml2::XMLElement* aElement)
{
	std::string type;
	aReader.ForceReadAttribute(aElement, "type", type);

	tinyxml2::XMLElement* element = aReader.ForceFindFirstChild(aElement, "position");
	CU::Vector3<float> position;
	aReader.ForceReadAttribute(element, "X", position.x);
	aReader.ForceReadAttribute(element, "Y", position.y);
	aReader.ForceReadAttribute(element, "Z", position.z);
	position *= 10.f;

	element = aReader.FindFirstChild(aElement, "rotation");
	CU::Vector3<float> rotation;
	if (element != nullptr)
	{
		aReader.ForceReadAttribute(element, "X", rotation.x);
		aReader.ForceReadAttribute(element, "Y", rotation.y);
		aReader.ForceReadAttribute(element, "Z", rotation.z);
	}

	element = aReader.FindFirstChild(aElement, "scale");
	CU::Vector3<float> scale(1.f,1.f,1.f);
	if (element != nullptr)
	{
		aReader.ForceReadAttribute(element, "X", scale.x);
		aReader.ForceReadAttribute(element, "Y", scale.y);
		aReader.ForceReadAttribute(element, "Z", scale.z);
	}

	int powerUpCount = 0;
	std::string powerUpName;
	for (element = aReader.FindFirstChild(aElement, "powerup"); element != nullptr;
		element = aReader.FindNextElement(element, "powerup"))
	{
		DL_ASSERT_EXP(powerUpCount < 1, "To many powerups tried to be added to enemy from event.");
		if (element != nullptr)
		{
			tinyxml2::XMLElement* powerup = aReader.FindFirstChild(element, "type");
			aReader.ForceReadAttribute(powerup, "powerup", powerUpName);
		}
		++powerUpCount;
	}

	myMessage = new SpawnEnemyMessage(type, position, rotation, scale, powerUpName);
}

SpawnEnemyAction::~SpawnEnemyAction()
{
	delete myMessage;
}

void SpawnEnemyAction::OnEnter()
{
	PostMaster::GetInstance()->SendMessage(*myMessage);
}


void SpawnEnemyAction::OnExit()
{
}

bool SpawnEnemyAction::Update()
{
	return true;
}