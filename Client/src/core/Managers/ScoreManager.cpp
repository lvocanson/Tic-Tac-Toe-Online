#include "ScoreManager.h"

ScoreManager::ScoreManager()
{
    m_PlayerScores = std::map<TicTacToe::Piece, unsigned int>();
}

ScoreManager::~ScoreManager()
{
    Clear();
}

void ScoreManager::Clear()
{
    for (auto& playerScore : m_PlayerScores)
    {
        playerScore.second = 0;
    }

    m_PlayerScores.clear();
}

void ScoreManager::InitPlayerScores(const std::vector<Player*>& allPlayers)
{
    for (auto& player : allPlayers)
    {
        m_PlayerScores.insert({ player->GetPiece(), 0 });
    }
}

void ScoreManager::AddScoreToPlayer(TicTacToe::Piece piece)
{
    if (!IsScoreExists(piece)) return;

    m_PlayerScores[piece]++;
}

unsigned int ScoreManager::GetPlayerScore(TicTacToe::Piece piece)
{
    if (!IsScoreExists(piece)) return 0;

    return m_PlayerScores[piece];
}

bool ScoreManager::IsScoreExists(TicTacToe::Piece& piece)
{
    if (!m_PlayerScores.contains(piece))
    {
        DebugLog("Score isn't created for this player");
        return false;
    }

    return true;
}

