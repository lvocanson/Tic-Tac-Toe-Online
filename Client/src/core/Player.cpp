#include "Player.h"
#include <game/IDGenerator.h>

Player::Player()
{
    m_Data = {
        .Name = "DefaultName",
        .Piece = TicTacToe::Piece::Empty,
        .Color = sf::Color::White,
    };
}

Player::Player(const std::string& name, const sf::Color color, const TicTacToe::Piece piece)
{
    m_Data = {
        .Name = name,
        .Piece = piece,
        .Color = color,
    };
}

Player::~Player()
{
}

