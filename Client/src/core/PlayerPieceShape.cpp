#include "PlayerPieceShape.h"

PlayerPieceShape::PlayerPieceShape(TicTacToe::Player* player)
{
    m_PlayerID = player->GetPlayerID();
}

PlayerCrossShape::PlayerCrossShape(TicTacToe::Player* player) : PlayerPieceShape(player), sf::RectangleShape(sf::Vector2f(DEFAULT_PIECE_SIZE * 0.5f, DEFAULT_PIECE_SIZE * 0.5f))
{
    setFillColor(sf::Color::Transparent);
    setOutlineThickness(OUTLINE_THICKNESS);
    setOutlineColor(sf::Color::Color(255, 194, 0));
    setOrigin(getSize().x * 0.5f, getSize().y * 0.5f);
}

PlayerCircleShape::PlayerCircleShape(TicTacToe::Player* player) : sf::CircleShape(DEFAULT_PIECE_SIZE * 0.25f), PlayerPieceShape(player)
{
    setFillColor(sf::Color::Transparent);
    setOutlineThickness(OUTLINE_THICKNESS);
    setOutlineColor(sf::Color::Color(250, 92, 12));
    setOrigin(getRadius(), getRadius());
}
