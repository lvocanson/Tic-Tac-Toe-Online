#include "Player.h"

#include "IDGenerator.h"
#include "../Client/src/core/CoreDefinitions.h"

namespace TicTacToe
{
	Player::Player() : m_Name("DefaultName")
	{
		m_PlayerID = IDGenerator::GenerateID();
	}

	Player::Player(std::string name) : m_Name(std::move(name))
	{
		m_PlayerID = IDGenerator::GenerateID();
	}

	Player::~Player()
	{
	}
}
