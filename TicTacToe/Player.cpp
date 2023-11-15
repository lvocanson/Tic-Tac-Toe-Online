#include "Player.h"
#include "IDGenerator.h"

namespace TicTacToe
{
	Player::Player()
	{
        const int id = IDGenerator::GenerateID();
		m_PlayerData = {
			.PlayerID = id,
			.Name = "Player " + std::to_string(id)
		};
	}

	Player::Player(const std::string& name)
	{
		m_PlayerData = {
			.PlayerID = IDGenerator::GenerateID(),
			.Name = name
		};
	}

	Player::~Player()
    = default;
}
