#include "stdafx.h"
#include "GameStateMessage.h"

GameStateMessage::GameStateMessage(eGameState aGameState)
	: myGameState(aGameState)
{
	myMessageType = eMessageType::GAME_STATE;
}

GameStateMessage::GameStateMessage(eGameState aGameState, std::string aFilePath, bool aUseXML)
	: myGameState(aGameState)
	, myFilePath(aFilePath)
	, myUseXML(aUseXML)
{
	myMessageType = eMessageType::GAME_STATE;
}
