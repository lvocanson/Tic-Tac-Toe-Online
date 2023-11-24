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
    void InitPlayerTurn(const std::string starter);

    std::array<Player, 2>& GetAllPlayers() { return m_RegisteredPlayers; }
    const Player& GetPlayer(int index) { return m_RegisteredPlayers[index]; }
    const Player& GetPlayer(TicTacToe::Piece);

    Player* GetCurrentPlayer() { return m_CurrentPlayer; }
    Player* GetOpponentPlayer() { return m_OpponentPlayer; }

private:

    std::array<Player, 2> m_RegisteredPlayers;
    Player* m_CurrentPlayer;
    Player* m_OpponentPlayer;

    bool m_IsPlayerXTurn;
};
