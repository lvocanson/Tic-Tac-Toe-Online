#include "GameHistoryManager.h"


GameHistoryManager::GameHistoryManager()
{
    m_GameHistory = std::vector<GameData*>();
}

GameHistoryManager::~GameHistoryManager()
{
    Clear();
}


void GameHistoryManager::Clear()
{
    for (auto game : m_GameHistory)
    {
        RELEASE(game)
    }

    m_GameHistory.clear();
}

GameData* GameHistoryManager::GetGameData(unsigned int gameID) const
{
    if (gameID > m_GameHistory.size())
    {
        DebugLog("Invalid game id");
        return nullptr;
    }

    return m_GameHistory[--gameID];
}
