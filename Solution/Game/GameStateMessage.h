#pragma once
#include "Message.h"

enum class eGameState
{
	LOAD_LEVEL,
	CHANGE_LEVEL,
	RELOAD_LEVEL,
	LOAD_MENU
};

class GameStateMessage : public Message
{
public:
	GameStateMessage(eGameState aGameState);
	GameStateMessage(eGameState aGameState, std::string aFilePath);
	GameStateMessage(eGameState aGameState, const int& anID);

	const eGameState& GetGameState() const;
	const std::string& GetFilePath() const;
	const int GetID() const;

private:

	eGameState myGameState;
	std::string myFilePath;
	int myID;
};

inline const eGameState& GameStateMessage::GetGameState() const
{
	return myGameState;
}

inline const std::string& GameStateMessage::GetFilePath() const
{
	return myFilePath;
}

inline const int GameStateMessage::GetID() const
{
	return myID;
}