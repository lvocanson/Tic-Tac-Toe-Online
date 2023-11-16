#pragma once

class Player;

class PlayerPieceShape
{
public:

    PlayerPieceShape(const Player* player);

    TicTacToe::PieceID GetPlayerID() const { return m_ID; }

protected:

    TicTacToe::PieceID m_ID;

};

class PlayerCrossShape final : public PlayerPieceShape, public sf::RectangleShape
{
public:

    PlayerCrossShape(const Player* player);

};

class PlayerCircleShape final : public sf::CircleShape, public PlayerPieceShape
{
public:

    PlayerCircleShape(const Player* player);
};
