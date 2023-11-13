#include "ScoreManager.h"

TicTacToe::PlayerMove::PlayerMove(int playerID, int boardCell)
{
    PlayerID = playerID;
    BoardCell = boardCell;
}

TicTacToe::GameData::GameData(PlayerData* winner, const std::vector<PlayerMove*>& allMoves)
{
    Winner = winner;
    AllMoves = allMoves;
}

TicTacToe::GameData::~GameData()
{
    for (auto move : AllMoves)
    {
        delete move;
        move = nullptr;
    }

    AllMoves.clear();
}

TicTacToe::ScoreManager::ScoreManager()
{
    m_GameHistory = std::vector<GameData*>();
    m_CurrentGame = std::vector<PlayerMove*>();
}

TicTacToe::ScoreManager::~ScoreManager()
{
    for (auto game : m_GameHistory)
    {
        delete game;
        game = nullptr;
    }

    for (auto move : m_CurrentGame)
    {
        delete move;
        move = nullptr;
    }

    m_PlayerScores.clear();
    m_CurrentGame.clear();
    m_GameHistory.clear();
}

void TicTacToe::ScoreManager::RegisterPlayer(PlayerData* player)
{
    m_PlayerScores.insert(std::pair<int, int>(player->PlayerID, 0));
}

void TicTacToe::ScoreManager::AddMove(int playerID, int lastCellPlayed)
{
    m_CurrentGame.push_back(new PlayerMove(playerID, lastCellPlayed));
}

void TicTacToe::ScoreManager::AddScoreToPlayer(int playerID)
{
    m_PlayerScores[playerID]++;
}

int TicTacToe::ScoreManager::GetPlayerScore(int playerID)
{
    return m_PlayerScores[playerID];
}

void TicTacToe::ScoreManager::NewGame(PlayerData* winner)
{
    m_GameHistory.push_back(new GameData(winner, m_CurrentGame));
    ClearMoves();
}

void TicTacToe::ScoreManager::ClearMoves()
{
    m_CurrentGame.clear();
}
