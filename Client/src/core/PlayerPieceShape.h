#pragma once
#include "src/core/Managers/Resources/PlayerShapeRegistry.h"

class Player;

class PlayerPieceShape : public sf::Drawable
{
public:

    PlayerPieceShape(const TicTacToe::PieceID id, const PlayerShapeType type, const sf::Vector2f& position);

    TicTacToe::PieceID GetPlayerID() const { return m_ID; }
    PlayerShapeType GetPlayerShape() const { return m_ShapeType; }

private:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:

    PlayerShapeType m_ShapeType;
    TicTacToe::PieceID m_ID;
    sf::Vector2f m_Position;
};
