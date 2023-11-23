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

    void CreateNewPlayer(const std::string&, const sf::Color, const TicTacToe::Piece);
    void UnregisterPlayer(Player* player);

    const std::vector<Player*>& GetAllPlayers() { return m_RegisteredPlayers; }
    Player* GetPlayer(int index) const { return m_RegisteredPlayers[index]; }
    static Player* GetCurrentPlayer() { return m_CurrentPlayer; }
    int GetPlayerCount() const { return m_PlayerCount; }

private:

    std::vector<Player*> m_RegisteredPlayers;
    static Player* m_CurrentPlayer;

    int m_PlayerCount;
    int m_CurrentPlayerIndex;
};
