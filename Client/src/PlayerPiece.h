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

    PlayerCrossShape(TicTacToe::Player* player) : PlayerPiece(player), sf::RectangleShape(sf::Vector2f(50, 50))
    {
        setFillColor(sf::Color::Transparent);
        setOutlineThickness(10);
        setOutlineColor(sf::Color::Blue);
        setOrigin(getSize().x * 0.5f, getSize().y * 0.5f);
    }
};

class PlayerCircleShape final : public sf::CircleShape, public PlayerPiece
{
public:

    PlayerCircleShape(TicTacToe::Player* player) : sf::CircleShape(30), PlayerPiece(player)
    {
        setFillColor(sf::Color::Transparent);
        setOutlineThickness(10);
        setOutlineColor(sf::Color::Red);
        setOrigin(getRadius(), getRadius());
    }
};

