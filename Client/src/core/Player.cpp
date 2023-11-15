#include "Player.h"
#include "IDGenerator.h"


Player::Player()
{
    const TicTacToe::PieceID id = IDGenerator::GenerateID();

	m_Data = {
		.Name = "Player " + std::to_string(id),
		.Id = id,
		.Color = sf::Color::Black,
	};
}

Player::Player(const std::string& name, const sf::Color color)
{
	m_Data = {
		.Name = name,
		.Id = IDGenerator::GenerateID(),
		.Color = color,
	};
}

Player::~Player()
{
}
