#include "GameHistoryManager.h"

GameData::GameData(const PlayerData* winner, const std::vector<const PlayerMove*>* allMoves)
{
    Winner = winner;
    AllMoves = allMoves;
}

GameData::~GameData()
{
    Winner = nullptr;

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

void GameHistoryManager::SaveGame(const PlayerData* winner, const std::vector<const PlayerMove*>* playerMoves)
{
    m_GameHistory.push_back(new GameData(winner, playerMoves));
}

GameData* GameHistoryManager::GetGameData(unsigned int gameID)
{
    if (gameID < 0 || gameID >= m_GameHistory.size())
    {
        DebugLog("Invalid game id");
        return nullptr;
    }

    return m_GameHistory[gameID];
}
