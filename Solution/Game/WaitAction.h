#pragma once
#include "Action.h"

namespace tinyxml2
{
	class XMLElement;
}

namespace CommonUtilities
{
	class TimerManager;
}

class XMLReader;

class WaitAction : public Action
{
public:
	WaitAction(XMLReader& aReader, tinyxml2::XMLElement* aElement);
	~WaitAction();

	void OnEnter() override;
	void OnExit() override;
	bool Update() override;

private:
	void operator=(WaitAction&) = delete;

	const CommonUtilities::TimerManager& myTimerManager;
	float myStartTime;
	float myTime;
};

