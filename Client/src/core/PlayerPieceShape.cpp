#include "PlayerPieceShape.h"
#include "Player.h"

PlayerPieceShape::PlayerPieceShape(const TicTacToe::PieceID id, const sf::Vector2f& position)
{
    m_ID = id;
    m_Position = position;
}

void PlayerPieceShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform.translate(m_Position);
    target.draw(*PlayerShapeRegistry::GetPlayerShape(m_ID), states);
}

