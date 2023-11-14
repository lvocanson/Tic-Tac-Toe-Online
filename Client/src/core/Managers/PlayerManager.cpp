#include "PlayerManager.h"

TicTacToe::PlayerManager::PlayerManager()
{
}

TicTacToe::PlayerManager::~PlayerManager()
{
}

void TicTacToe::PlayerManager::Init()
{
    m_PlayerCount = 0;
    m_RegisteredPlayers = std::vector<Player*>();
    m_CurrentPlayerIndex = 0;
}

void TicTacToe::PlayerManager::Clear()
{
    for (auto player : m_RegisteredPlayers)
    {
        RELEASE(player)
    }

    m_CurrentPlayer = nullptr;

    m_RegisteredPlayers.clear();
}

void TicTacToe::PlayerManager::SwitchPlayerTurn()
{
    if (m_CurrentPlayerIndex == m_PlayerCount - 1)
    {
        m_CurrentPlayer = m_RegisteredPlayers[0];
    }
    else
    {
        m_CurrentPlayerIndex++;
        m_CurrentPlayer = m_RegisteredPlayers[m_CurrentPlayerIndex];
    }
}

void TicTacToe::PlayerManager::RegisterNewPlayer(std::string name)
{
    Player* newPlayer = new Player(name);
    m_RegisteredPlayers.push_back(newPlayer);

    m_PlayerCount++;

    if (m_CurrentPlayer == nullptr)
    {
        m_CurrentPlayer = newPlayer;
    }
}

void TicTacToe::PlayerManager::UnregisterPlayer(Player* player)
{
    if (m_CurrentPlayer == player)
    {
        SwitchPlayerTurn();
    }

    std::erase(m_RegisteredPlayers, player);
    m_PlayerCount--;
}
