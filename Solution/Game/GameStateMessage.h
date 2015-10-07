#pragma once
#include "Message.h"

enum class eGameState
{
	MAIN_MENU_STATE,
	LEVEL_SELECT_STATE,
	INGAME_STATE
};

class GameStateMessage : public Message
{
public:
	GameStateMessage(eGameState aGameState);
	GameStateMessage(eGameState aGameState, std::string aFilePath, bool aUseXML = true);

	const eGameState& GetGameState() const;
	const std::string& GetFilePath() const;

	bool myUseXML; //temp
private:

	eGameState myGameState;
	std::string myFilePath;
};

inline const eGameState& GameStateMessage::GetGameState() const
{
	return myGameState;
}

inline const std::string& GameStateMessage::GetFilePath() const
{
	return myFilePath;
}