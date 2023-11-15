#include "ScoreManager.h"

TicTacToe::GameData::GameData(const PlayerData* winner, const std::vector<PlayerMove>& allMoves)
{
    Winner = winner;
    AllMoves = allMoves;
}

TicTacToe::GameData::~GameData()
{
    AllMoves.clear();
}

TicTacToe::ScoreManager::ScoreManager()
{
    m_GameHistory = std::vector<GameData*>();
    m_CurrentGame = std::vector<PlayerMove>();
    m_PlayerScores = std::map<PieceID, unsigned int>();
}

TicTacToe::ScoreManager::~ScoreManager()
{
    Clear();
}

void TicTacToe::ScoreManager::Init()
{
    
}

void TicTacToe::ScoreManager::Clear()
{
    for (auto game : m_GameHistory)
    {
        RELEASE(game)
    }

    m_PlayerScores.clear();
    m_CurrentGame.clear();
    m_GameHistory.clear();
}

void TicTacToe::ScoreManager::CreateScoreForPlayer(PlayerData* playerData, Window* window)
{
    m_PlayerScores.insert(std::pair<int, int>(playerData->Id, 0));
}

void TicTacToe::ScoreManager::AddPlayerMove(PieceID pieceID, unsigned int lastCellPlayed)
{
    const PlayerMove playerMove = {
        .PieceID = pieceID,
        .BoardCell = lastCellPlayed
    };

    m_CurrentGame.push_back(playerMove);
}

void TicTacToe::ScoreManager::AddScoreToPlayer(const PlayerData* player)
{
    const PieceID id = player->Id;

    m_PlayerScores[id]++;
}

unsigned int TicTacToe::ScoreManager::GetPlayerScore(PieceID pieceID)
{
    return m_PlayerScores[pieceID];
}

void TicTacToe::ScoreManager::SaveGame(const PlayerData* winner)
{
    m_GameHistory.push_back(new GameData(winner, m_CurrentGame));
    ClearMoves();
}

void TicTacToe::ScoreManager::ClearMoves()
{
    m_CurrentGame.clear();
}
