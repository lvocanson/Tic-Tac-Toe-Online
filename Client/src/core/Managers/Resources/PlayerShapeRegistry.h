#pragma once

enum PlayerShapeType
{
    Square,
    Circle,
};

class PlayerShapeRegistry
{
public:

    static sf::Drawable* GetPlayerShape(TicTacToe::Piece piece);
    static void ClearPlayerShapes();
    static sf::Color GetPlayerColor(TicTacToe::Piece piece);
private:

    friend class PlayerManager;
    static void CreatePlayerShape(TicTacToe::Piece, sf::Color);

    inline static std::unordered_map<TicTacToe::Piece, sf::Shape*> m_PlayerShapes = {};

};
