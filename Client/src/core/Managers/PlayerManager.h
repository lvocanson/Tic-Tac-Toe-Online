#pragma once
#include "src/core/Player.h"
#include "IManager.h"

class PlayerManager : public IManager
{
public:

    PlayerManager();
    ~PlayerManager() override;

    void Init() override;
    void Clear() override;

    void SwitchPlayerTurn();

    void CreateNewPlayer(const std::string& name, const sf::Color color, const PlayerShapeType shapeType);
    void UnregisterPlayer(Player* player);

    // TODO: rework this
    bool IsPlayerOneTurn() const { return m_IsPlayerOneTurn; }

    const std::vector<Player*>& GetAllPlayers() { return m_RegisteredPlayers; }
    Player* GetPlayer(int index) const { return m_RegisteredPlayers[index]; }
    static Player* GetCurrentPlayer() { return m_CurrentPlayer; }
    int GetPlayerCount() const { return m_PlayerCount; }

private:

    std::vector<Player*> m_RegisteredPlayers;
    static Player* m_CurrentPlayer;

    int m_PlayerCount;
    int m_CurrentPlayerIndex;

    // TODO: rework this
    bool m_IsPlayerOneTurn;
};
