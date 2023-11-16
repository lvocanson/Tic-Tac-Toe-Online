#pragma once

enum PlayerShapeType
{
    Square,
    Circle,
    //Cross
};

class PlayerShapeRegistry
{
public:

    static sf::Drawable* GetPlayerShape(TicTacToe::PieceID);

private:

    friend class PlayerManager;
    static void CreatePlayerShape(PlayerShapeType, TicTacToe::PieceID, sf::Color);

    inline static std::unordered_map<TicTacToe::PieceID, sf::Drawable*> m_PlayerShapes = {};

};

