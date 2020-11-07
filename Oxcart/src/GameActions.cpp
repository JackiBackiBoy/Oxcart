#include "GameActions.h"
#include "GameInfo.h"

void GameActions::ContinueGame()
{
	GameInfo::myCurrentGameState = GameState::Playing;
}