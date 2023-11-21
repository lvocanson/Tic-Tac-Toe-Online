#include "GameHistoryManager.h"

GameHistoryManager::GameHistoryManager()
{
	m_GameHistory = std::vector<GameData*>();
}

GameHistoryManager::~GameHistoryManager()
{
	Clear();
}

void GameHistoryManager::Init()
{

}

void GameHistoryManager::Clear()
{
	for (auto game : m_GameHistory)
	{
		RELEASE(game)
	}

	m_GameHistory.clear();
}

void GameHistoryManager::SaveGame(const PlayerData* winner)
{
	m_GameHistory.push_back(new GameData(winner, m_CurrentGame));
	ClearMoves();
}


void GameHistoryManager::ClearMoves()
{
	m_CurrentGame.clear();
}