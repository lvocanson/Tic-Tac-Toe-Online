#include "src/pch.h"
#include "PlayerPiece.h"

PlayerPiece::PlayerPiece(TicTacToe::Player* player)
{
    m_PlayerID = player->GetPlayerID();
}

PlayerCrossShape::PlayerCrossShape(TicTacToe::Player* player) : PlayerPiece(player), sf::RectangleShape(sf::Vector2f(PIECE_SIZE * 0.5f, PIECE_SIZE * 0.5f))
{
    setFillColor(sf::Color::Transparent);
    setOutlineThickness(OUTLINE_THICKNESS);
    setOutlineColor(sf::Color::Blue);
    setOrigin(getSize().x * 0.5f, getSize().y * 0.5f);
}

PlayerCircleShape::PlayerCircleShape(TicTacToe::Player* player) : sf::CircleShape(PIECE_SIZE * 0.25f), PlayerPiece(player)
{
    setFillColor(sf::Color::Transparent);
    setOutlineThickness(OUTLINE_THICKNESS);
    setOutlineColor(sf::Color::Red);
    setOrigin(getRadius(), getRadius());
}