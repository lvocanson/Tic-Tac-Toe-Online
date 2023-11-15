#include "PlayerPieceShape.h"
#include "Player.h"

PlayerPieceShape::PlayerPieceShape(const Player* player)
{
    m_ID = player->GetPlayerID();
}

PlayerCrossShape::PlayerCrossShape(const Player* player) : PlayerPieceShape(player), RectangleShape(sf::Vector2f(DEFAULT_PIECE_SIZE * 0.5f, DEFAULT_PIECE_SIZE * 0.5f))
{
    setFillColor(sf::Color::Transparent);
    setOutlineThickness(OUTLINE_THICKNESS);
    setOutlineColor(sf::Color::Color(255, 194, 0));
    setOrigin(getSize().x * 0.5f, getSize().y * 0.5f);
}

PlayerCircleShape::PlayerCircleShape(const Player* player) : CircleShape(DEFAULT_PIECE_SIZE * 0.25f), PlayerPieceShape(player)
{
    setFillColor(sf::Color::Transparent);
    setOutlineThickness(OUTLINE_THICKNESS);
    setOutlineColor(sf::Color::Color(250, 92, 12));
    setOrigin(getRadius(), getRadius());
}
