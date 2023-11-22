#pragma once

enum PlayerShapeType
{
    Square,
    Circle,
};

class PlayerShapeRegistry
{
public:

    static sf::Drawable* GetPlayerShape(PlayerShapeType);
    static void ClearPlayerShapes();

private:

    friend class PlayerManager;
    static void CreatePlayerShape(PlayerShapeType, TicTacToe::PieceID, sf::Color);

    inline static std::unordered_map<PlayerShapeType, sf::Drawable*> m_PlayerShapes = {};

};
