#include "GameHistoryManager.h"

GameData::GameData(const std::vector<const PlayerMove*>* allMoves)
{
    AllMoves = allMoves;
}

GameData::~GameData()
{
    for (auto move : *AllMoves)
    {
        RELEASE(move)
    }

    RELEASE(AllMoves)
}

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

void GameHistoryManager::SaveGame(const std::vector<const PlayerMove*>* playerMoves)
{
    m_GameHistory.push_back(new GameData(playerMoves));
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
