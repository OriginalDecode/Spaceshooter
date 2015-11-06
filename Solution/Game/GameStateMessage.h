#pragma once
#include "Message.h"

enum class eGameState
{
	LOAD_GAME,
	RELOAD_LEVEL,
	LOAD_MENU,
	COMPLETE_LEVEL,
	LOAD_NEXT_LEVEL,
	MOUSE_LOCK,
	EXIT_GAME
};

class GameStateMessage : public Message
{
public:
	GameStateMessage(eGameState aGameState);
	GameStateMessage(eGameState aGameState, const std::string& aFilePath);
	GameStateMessage(eGameState aGameState, const int& anID);
	GameStateMessage(eGameState aGameState, const bool& anIsMouseLocked);

	const eGameState& GetGameState() const;
	const std::string& GetFilePath() const;
	const int GetID() const;
	const bool& GetMouseLocked() const;

private:

	eGameState myGameState;
	std::string myFilePath;
	int myID;
	bool myMouseIsLocked; // temp
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

inline const bool& GameStateMessage::GetMouseLocked() const
{
	return myMouseIsLocked;
}