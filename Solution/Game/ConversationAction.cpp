#include "stdafx.h"
#include "PostMaster.h"
#include "ConversationAction.h"
#include "ConversationMessage.h"
#include <XMLReader.h>


ConversationAction::ConversationAction(XMLReader& aReader, tinyxml2::XMLElement* aElement, ConversationManager& aConversationManager)
	: myConversationManager(aConversationManager)
{
	tinyxml2::XMLElement* element = aReader.ForceFindFirstChild(aElement, "conversation");

	for (tinyxml2::XMLElement* element = aReader.ForceFindFirstChild(aElement, "sentence"); element != nullptr;
		element = aReader.FindNextElement(aElement, "sentence"))
	{
		std::string text;
		aReader.ForceReadAttribute(element, "text", text);

		mySentences.Add(new Sentence(text));
	}

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
	CU::Vector3<float> scale(1.f, 1.f, 1.f);
	if (element != nullptr)
	{
		aReader.ForceReadAttribute(element, "X", scale.x);
		aReader.ForceReadAttribute(element, "Y", scale.y);
		aReader.ForceReadAttribute(element, "Z", scale.z);
	}
	//myMessage = new ConversationMessage(type, position, rotation, scale);
}

ConversationAction::~ConversationAction()
{
	delete myMessage;
}

void ConversationAction::OnEnter()
{
	//PostMaster::GetInstance()->SendMessage(*myMessage);
}


bool ConversationAction::OnExit()
{
	return true;
}

bool ConversationAction::Update()
{
	return true;
}