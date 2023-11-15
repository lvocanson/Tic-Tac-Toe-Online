#pragma once
#include "Player.h"


class PlayerPieceShape
{
public:

    PlayerPieceShape(TicTacToe::Player* player);

    int GetPlayerID() const { return m_ID; }

protected:

    int m_ID;

};

class PlayerCrossShape final : public PlayerPieceShape, public sf::RectangleShape
{
public:

    PlayerCrossShape(Player* player);

};

class PlayerCircleShape final : public sf::CircleShape, public PlayerPieceShape
{
public:

    PlayerCircleShape(Player* player);
};


