#include "src/pch.h"
#include "PlayerShapeRegistry.h"
#include <SFML/Graphics.hpp>


sf::Drawable* PlayerShapeRegistry::GetPlayerShape(PlayerShapeType shapeType)
{
    return m_PlayerShapes[shapeType];
}

void PlayerShapeRegistry::ClearPlayerShapes()
{
   for (auto& shape : m_PlayerShapes)
    {
        delete shape.second;
    }

    m_PlayerShapes.clear();
}

void PlayerShapeRegistry::CreatePlayerShape(PlayerShapeType shapeType, TicTacToe::PieceID id, sf::Color color)
{
    switch (shapeType)
    {
    case Square:
    {
        auto shape = new sf::RectangleShape;
        shape->setSize(sf::Vector2f(DEFAULT_PIECE_SIZE * 0.5f, DEFAULT_PIECE_SIZE * 0.5f));
        shape->setFillColor(sf::Color::Transparent);
        shape->setOutlineThickness(OUTLINE_THICKNESS);
        shape->setOutlineColor(color);
        shape->setOrigin(shape->getSize().x * 0.5f, shape->getSize().y * 0.5f);
        m_PlayerShapes.insert(std::make_pair(Square, shape));

        break;
    }
     
    case Circle:
        auto shape = new sf::CircleShape;
        shape->setRadius(DEFAULT_PIECE_SIZE * 0.25f);
        shape->setFillColor(sf::Color::Transparent);
        shape->setOutlineThickness(OUTLINE_THICKNESS);
        shape->setOutlineColor(color);
        shape->setOrigin(shape->getRadius(), shape->getRadius());
        m_PlayerShapes.insert(std::make_pair(Circle, shape));

        break;
    }
}
