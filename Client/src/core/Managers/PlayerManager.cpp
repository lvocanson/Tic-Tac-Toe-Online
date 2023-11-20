#include "PlayerManager.h"

Player* PlayerManager::m_CurrentPlayer = nullptr;

PlayerManager::PlayerManager() : m_PlayerCount(0), m_CurrentPlayerIndex(0), m_IsPlayerOneTurn(true)
{
    m_RegisteredPlayers = std::vector<Player*>();
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
    for (auto player : m_RegisteredPlayers)
    {
        RELEASE(player)
    }

    m_CurrentPlayer = nullptr;

    m_RegisteredPlayers.clear();
}

void PlayerManager::SwitchPlayerTurn()
{
    m_CurrentPlayerIndex++;

    if (m_CurrentPlayerIndex >= m_PlayerCount)
    {
        m_CurrentPlayerIndex = 0;
    }

    m_CurrentPlayer = m_RegisteredPlayers[m_CurrentPlayerIndex];

    m_IsPlayerOneTurn = !m_IsPlayerOneTurn;
}

void PlayerManager::CreateNewPlayer(const std::string& name, const sf::Color color, const PlayerShapeType shapeType)
{
    const auto newPlayer = new Player(name, color, shapeType);

    m_RegisteredPlayers.push_back(newPlayer);
    m_PlayerCount++;

    if (m_CurrentPlayer == nullptr)
    {
        m_CurrentPlayer = newPlayer;
    }

    PlayerShapeRegistry::CreatePlayerShape(shapeType, newPlayer->GetPlayerID(), color);
}

void PlayerManager::UnregisterPlayer(Player* player)
{
    if (m_CurrentPlayer == player)
    {
        SwitchPlayerTurn();
    }

    std::erase(m_RegisteredPlayers, player);
    m_PlayerCount--;
}
