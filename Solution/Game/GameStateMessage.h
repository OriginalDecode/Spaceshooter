#pragma once
#include "Message.h"

enum class eGameState
{
	MAIN_MENU_STATE,
	LEVEL_SELECT_STATE,
	INGAME_STATE,
};

class GameStateMessage : public Message
{
public:
	GameStateMessage(eGameState aGameState);

	const eGameState& GetGameState() const;

private:

	eGameState myGameState;
};

inline const eGameState& GameStateMessage::GetGameState() const
{
	return myGameState;
}