#pragma once
#include "Player.h"


class PlayerPieceShape
{
public:

    PlayerPieceShape(TicTacToe::Player* player);

    int GetPlayerID() const { return m_PlayerID; }

protected:

    int m_PlayerID;

};

class PlayerCrossShape final : public PlayerPieceShape, public sf::RectangleShape
{
public:

    PlayerCrossShape(TicTacToe::Player* player);

};

class PlayerCircleShape final : public sf::CircleShape, public PlayerPieceShape
{
public:

    PlayerCircleShape(TicTacToe::Player* player);
};


