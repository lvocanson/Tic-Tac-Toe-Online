#pragma once
#include <string>
#include "Managers/Resources/PlayerShapeRegistry.h"
#include "game/GameData.h"

struct PlayerData
{
    std::string Name;
    TicTacToe::Piece Piece;
    sf::Color Color;
};

class Player
{
public:

    Player();
    ~Player();

    bool IsInited() { return m_Data.Piece != TicTacToe::Piece::Empty; }

    void SetData(const std::string& name, const sf::Color color, const TicTacToe::Piece);
    void SetColor(const sf::Color color) { m_Data.Color = color; }

    sf::Color GetColor() const { return m_Data.Color; }
    TicTacToe::Piece GetPiece() const { return m_Data.Piece; }
    std::string GetName() const { return m_Data.Name; }
    PlayerData* GetData() { return &m_Data; }

    void SetName(const std::string& name) { m_Data.Name = name; }

private:

    PlayerData m_Data;
};
