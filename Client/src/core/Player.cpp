#include "Player.h"
#include "IDGenerator.h"


Player::Player()
{
    const TicTacToe::PieceID id = IDGenerator::GenerateID();

	m_Data = {
		.Name = "Player " + id,
		.Id = id,
	};
}

Player::Player(const std::string& name)
{
	m_Data = {
		.Name = name,
		.Id = IDGenerator::GenerateID(),
	};
}

Player::~Player()
{
}
