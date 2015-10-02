#include "stdafx.h"
#include "GameStateMessage.h"

GameStateMessage::GameStateMessage(eGameState aGameState)
	: myGameState(aGameState)
{
	myMessageType = eMessageType::GAME_STATE;
}
