#include "stdafx.h"
#include "ConversationAction.h"
#include "ConversationManager.h"

ConversationAction::ConversationAction(const std::string& aName, ConversationManager& aConversationManager)
	: myName(aName)
	, myConversationManager(aConversationManager)
{
}

ConversationAction::~ConversationAction()
{
}

void ConversationAction::OnEnter()
{
	myConversationManager.Start(myName);
}


bool ConversationAction::OnExit()
{
	return true;
}

bool ConversationAction::Update()
{
	return myConversationManager.Update();
}