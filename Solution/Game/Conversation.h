#pragma once
#include <string>
#include <GrowingArray.h>

namespace tinyxml2
{
	class XMLElement;
}
class XMLReader;

class Conversation
{
public:
	Conversation(const std::string& aName, XMLReader& aReader, tinyxml2::XMLElement* aElement);
	~Conversation();

	void Start();
	bool Update();

private:
	struct Sentence
	{
		Sentence(const std::string& aText) : myText(aText){}
		const std::string myText;
	};

	void SetCurrentSentence();

	CU::GrowingArray<Sentence*> mySentences;

	int myCurrentSentence;
	float myTimeLeft;
	float myShowTime;
};

