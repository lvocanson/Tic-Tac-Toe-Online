#include "PlayerManager.h"

PlayerManager::PlayerManager()
{
}

PlayerManager::~PlayerManager()
{
    Clear();
}

void PlayerManager::Init()
{

}

void PlayerManager::Clear()
{
    m_CurrentPlayer = nullptr;
    m_OpponentPlayer = nullptr;
}

void PlayerManager::SwitchPlayerTurn()
{
    m_IsPlayerXTurn = !m_IsPlayerXTurn;

    m_CurrentPlayer = &m_RegisteredPlayers[m_IsPlayerXTurn];
    m_OpponentPlayer = &m_RegisteredPlayers[!m_IsPlayerXTurn];
}

void PlayerManager::CreateNewPlayer(const std::string& name, const sf::Color color, const TicTacToe::Piece piece)
{
    if (m_RegisteredPlayers[0].IsInited())
        m_RegisteredPlayers[0].SetData(name, color, piece);
    else
        m_RegisteredPlayers[1].SetData(name, color, piece);

    PlayerShapeRegistry::CreatePlayerShape(piece, color);
}

void PlayerManager::InitPlayerTurn(const std::string starter)
{
    if (starter == m_RegisteredPlayers[0].GetName())
    {
        m_CurrentPlayer = &m_RegisteredPlayers[0];
        m_OpponentPlayer = &m_RegisteredPlayers[1];
    }
    else
    {
        m_CurrentPlayer = &m_RegisteredPlayers[1];
        m_OpponentPlayer = &m_RegisteredPlayers[0];
    }

    m_IsPlayerXTurn = m_CurrentPlayer->GetPiece() == TicTacToe::Piece::X;
}

const Player& PlayerManager::GetPlayer(TicTacToe::Piece piece)
{
    for (auto player : m_RegisteredPlayers)
    {
        if (player.GetPiece() == piece)
        {
            return player;
        }
    }
}
