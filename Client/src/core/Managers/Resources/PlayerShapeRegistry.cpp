#include "src/pch.h"
#include "PlayerShapeRegistry.h"
#include <SFML/Graphics.hpp>

sf::Drawable* PlayerShapeRegistry::GetPlayerShape(TicTacToe::Piece piece)
{
    return m_PlayerShapes.at(piece);
}

void PlayerShapeRegistry::ClearPlayerShapes()
{
    for (auto& shape : m_PlayerShapes)
    {
        RELEASE(shape.second);
    }

    m_PlayerShapes.clear();
}

sf::Color PlayerShapeRegistry::GetPlayerColor(TicTacToe::Piece piece)
{
    return m_PlayerShapes.at(piece)->getOutlineColor();
}

void PlayerShapeRegistry::CreatePlayerShape(TicTacToe::Piece piece, sf::Color color)
{
    if (m_PlayerShapes.contains(piece))
    {
        return;
    }

    switch (piece)
    {
    case TicTacToe::Piece::X:
    {
        auto shape = new sf::RectangleShape;
        shape->setSize(sf::Vector2f(DEFAULT_PIECE_SIZE * 0.5f, DEFAULT_PIECE_SIZE * 0.5f));
        shape->setFillColor(sf::Color::Transparent);
        shape->setOutlineThickness(OUTLINE_THICKNESS);
        shape->setOutlineColor(color);
        shape->setOrigin(shape->getSize().x * 0.5f, shape->getSize().y * 0.5f);
        m_PlayerShapes.insert({ piece, shape });

        break;
    }

    case TicTacToe::Piece::O:
        auto shape = new sf::CircleShape;
        shape->setRadius(DEFAULT_PIECE_SIZE * 0.25f);
        shape->setFillColor(sf::Color::Transparent);
        shape->setOutlineThickness(OUTLINE_THICKNESS);
        shape->setOutlineColor(color);
        shape->setOrigin(shape->getRadius(), shape->getRadius());
        m_PlayerShapes.insert({ piece, shape });

        break;
    }


}
