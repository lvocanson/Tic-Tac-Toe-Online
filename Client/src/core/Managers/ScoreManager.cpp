#include "ScoreManager.h"

GameData::GameData(const PlayerData* winner, const std::vector<PlayerMove>& allMoves)
{
    Winner = winner;
    AllMoves = allMoves;
}

GameData::~GameData()
{
    AllMoves.clear();
}

ScoreManager::ScoreManager()
{
    m_GameHistory = std::vector<GameData*>();
    m_CurrentGame = std::vector<PlayerMove>();
    m_PlayerScores = std::map<TicTacToe::PieceID, unsigned int>();
}

ScoreManager::~ScoreManager()
{
    Clear();
}

void ScoreManager::Init()
{
    
}

void ScoreManager::Clear()
{
    for (auto game : m_GameHistory)
    {
        RELEASE(game)
    }

    m_PlayerScores.clear();
    m_CurrentGame.clear();
    m_GameHistory.clear();
}

void ScoreManager::CreateScoreForPlayer(PlayerData* playerData, Window* window)
{
    m_PlayerScores.insert(std::pair<int, int>(playerData->Id, 0));
}

void ScoreManager::AddPlayerMove(TicTacToe::PieceID pieceID, unsigned int lastCellPlayed)
{
    const PlayerMove playerMove = {
        .PieceID = pieceID,
        .BoardCell = lastCellPlayed
    };

    m_CurrentGame.push_back(playerMove);
}

void ScoreManager::AddScoreToPlayer(TicTacToe::PieceID pieceID)
{
    if (!IsScoreExists(pieceID)) return;

    m_PlayerScores[pieceID]++;
}

unsigned int ScoreManager::GetPlayerScore(TicTacToe::PieceID pieceID)
{
    if (!IsScoreExists(pieceID)) return 0;

    return m_PlayerScores[pieceID];
}

bool ScoreManager::IsScoreExists(TicTacToe::PieceID& pieceID)
{
    if (!m_PlayerScores.contains(pieceID))
    {
        DebugLog("Score isn't created for this player");
        return false;
    }

    return true;
}

void ScoreManager::SaveGame(const PlayerData* winner)
{
    m_GameHistory.push_back(new GameData(winner, m_CurrentGame));
    ClearMoves();
}

void ScoreManager::ClearMoves()
{
    m_CurrentGame.clear();
}
