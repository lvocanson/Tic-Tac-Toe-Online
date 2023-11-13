#pragma once
#include "Player.h"

class PlayerPiece
{
public:

	PlayerPiece(TicTacToe::Player* player);

    int GetPlayerID() const { return m_PlayerID; }

protected:

    int m_PlayerID;

};

class PlayerCrossShape final : public PlayerPiece, public sf::RectangleShape
{
public:

    PlayerCrossShape(TicTacToe::Player* player);

};

class PlayerCircleShape final : public sf::CircleShape, public PlayerPiece
{
public:

    PlayerCircleShape(TicTacToe::Player* player);
};

