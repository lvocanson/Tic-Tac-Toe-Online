#pragma once
#include "Player.h"

class TicTacToe::Player;

class PlayerPiece
{
public:

	PlayerPiece(TicTacToe::Player* player);

    int GetPlayerID() const { return m_ID; }

protected:

    int m_ID;

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

