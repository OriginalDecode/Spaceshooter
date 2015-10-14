#include "stdafx.h"
#include "GameStateMessage.h"

GameStateMessage::GameStateMessage(eGameState aGameState)
	: myGameState(aGameState)
	, Message(eMessageType::GAME_STATE)
{
}

GameStateMessage::GameStateMessage(eGameState aGameState, std::string aFilePath)
	: myGameState(aGameState)
	, myFilePath(aFilePath)
	, Message(eMessageType::GAME_STATE)
{
}

GameStateMessage::GameStateMessage(eGameState aGameState, const int& anID)
	: myGameState(aGameState)
	, myID(anID)
	, Message(eMessageType::GAME_STATE)
{
}

GameStateMessage::GameStateMessage(eGameState aGameState, const bool& anIsMouseLocked)
	: myGameState(aGameState)
	, myMouseIsLocked(anIsMouseLocked)
	, Message(eMessageType::GAME_STATE)
{
}