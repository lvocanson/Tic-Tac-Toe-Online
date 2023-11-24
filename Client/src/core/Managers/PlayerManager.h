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
    void InitPlayerTurn(const std::string& starter, const TicTacToe::Piece& playerPiece);

    bool IsPlayerTurn() const { return m_IsPlayerTurn; }

    std::map<TicTacToe::Piece, Player>& GetAllPlayers() { return m_RegisteredPlayers; }
    Player& GetPlayerByPiece(TicTacToe::Piece);
    Player* GetCurrentPlayer() const { return m_CurrentPlayer; }
    Player* GetOpponentPlayer() const { return m_OpponentPlayer; }
    Player& GetOpponentPlayerByPiece(TicTacToe::Piece);

private:

    std::map<TicTacToe::Piece, Player> m_RegisteredPlayers;
    Player* m_CurrentPlayer;
    Player* m_OpponentPlayer;

    bool m_IsPlayerTurn;
    TicTacToe::Piece m_PlayerPiece;
};
