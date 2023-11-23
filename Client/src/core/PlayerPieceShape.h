#pragma once
#include "src/core/Managers/Resources/PlayerShapeRegistry.h"

class Player;

class PlayerPieceShape : public sf::Drawable
{
public:

    PlayerPieceShape(const TicTacToe::Piece id, const sf::Vector2f& position);

    TicTacToe::Piece GetPiece() const { return m_Piece; }

private:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:

    TicTacToe::Piece m_Piece;
    sf::Vector2f m_Position;
    sf::Drawable* m_Shape;
};
