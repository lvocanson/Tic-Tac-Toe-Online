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
        RELEASE(move)
    }

    AllMoves.clear();
}

TicTacToe::ScoreManager::~ScoreManager()
{
    for (auto game : m_GameHistory)
    {
        RELEASE(game)
    }

    for (auto move : m_CurrentGame)
    {
        RELEASE(move)
    }

    m_PlayerScores.clear();
    m_CurrentGame.clear();
    m_GameHistory.clear();
}

void TicTacToe::ScoreManager::Init()
{
    m_PlayerScores = std::map<int, int>();
    m_GameHistory = std::vector<GameData*>();
    m_CurrentGame = std::vector<PlayerMove*>();
}

void TicTacToe::ScoreManager::CreateScoreForPlayer(PlayerData* player)
{
    m_PlayerScores.insert(std::pair<int, int>(player->PlayerID, 0));
}

void TicTacToe::ScoreManager::AddPlayerMove(int playerID, int lastCellPlayed)
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

void TicTacToe::ScoreManager::InitNewGame(PlayerData* winner)
{
    m_GameHistory.push_back(new GameData(winner, m_CurrentGame));
    ClearMoves();
}

void TicTacToe::ScoreManager::ClearMoves()
{
    m_CurrentGame.clear();
}
