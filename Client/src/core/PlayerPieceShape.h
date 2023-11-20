#pragma once
#include "src/core/Managers/Resources/PlayerShapeRegistry.h"

class Player;

class PlayerPieceShape : public sf::Drawable
{
public:

    PlayerPieceShape(const TicTacToe::PieceID id, const sf::Vector2f& position);

    TicTacToe::PieceID GetPlayerID() const { return m_ID; }

private:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:

    TicTacToe::PieceID m_ID;
    sf::Vector2f m_Position;

};
