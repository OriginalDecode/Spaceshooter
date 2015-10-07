#include "stdafx.h"
#include "GameStateMessage.h"

GameStateMessage::GameStateMessage(eGameState aGameState)
	: myGameState(aGameState)
{
	myMessageType = eMessageType::GAME_STATE;
}

GameStateMessage::GameStateMessage(eGameState aGameState, std::string aFilePath)
	: myGameState(aGameState)
	, myFilePath(aFilePath)
{
	myMessageType = eMessageType::GAME_STATE;
}

GameStateMessage::GameStateMessage(eGameState aGameState, const int& anID)
	: myGameState(aGameState)
	, myID(anID)
{
	myMessageType = eMessageType::GAME_STATE;
}