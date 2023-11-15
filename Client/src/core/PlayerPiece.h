#pragma once
#include "Player.h"

class PlayerPiece
{
public:

	PlayerPiece(Player* player);

    int GetPlayerID() const { return m_ID; }

protected:

    int m_ID;

};

class PlayerCrossShape final : public PlayerPiece, public sf::RectangleShape
{
public:

    PlayerCrossShape(Player* player);

};

class PlayerCircleShape final : public sf::CircleShape, public PlayerPiece
{
public:

    PlayerCircleShape(Player* player);
};

