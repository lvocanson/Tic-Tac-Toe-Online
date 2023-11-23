#include "PlayerPieceShape.h"
#include "Player.h"

PlayerPieceShape::PlayerPieceShape(const TicTacToe::Piece piece, const sf::Vector2f& position)
{
    m_Piece = piece;
    m_Position = position;
    m_Shape = PlayerShapeRegistry::GetPlayerShape(m_Piece);
}

void PlayerPieceShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform.translate(m_Position);
    target.draw(*m_Shape, states);
}
