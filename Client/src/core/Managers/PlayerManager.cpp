#include "PlayerManager.h"

TicTacToe::PlayerManager::PlayerManager()
{
    m_PlayerCount = 0;
    m_RegisteredPlayers = std::vector<Player*>();
    m_CurrentPlayerIndex = 0;
    m_CurrentPlayer = nullptr;
}

TicTacToe::PlayerManager::~PlayerManager()
{
   Clear();
}

void TicTacToe::PlayerManager::Init()
{

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
    m_CurrentPlayerIndex++;

    if (m_CurrentPlayerIndex >= m_PlayerCount)
    {
        m_CurrentPlayerIndex = 0;
    }

    m_CurrentPlayer = m_RegisteredPlayers[m_CurrentPlayerIndex];

    m_IsPlayerOneTurn = !m_IsPlayerOneTurn;
}

void TicTacToe::PlayerManager::CreateNewPlayer(std::string name)
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
