#pragma once
#include "Action.h"
#include <GrowingArray.h>

namespace tinyxml2
{
	class XMLElement;
}
class ConversationMessage;
class XMLReader;
class ConversationManager;

class ConversationAction : public Action
{
public:
	ConversationAction(XMLReader& aReader, tinyxml2::XMLElement* aElement, ConversationManager& aConversationManager);
	~ConversationAction();

	void OnEnter() override;
	bool OnExit() override;
	bool Update() override;

private:
	struct Sentence
	{
		Sentence(const std::string& aText) : myText(aText){}
		const std::string myText;
	};

	ConversationManager& myConversationManager;
	ConversationMessage* myMessage;

	CU::GrowingArray<Sentence*> mySentences;
};

