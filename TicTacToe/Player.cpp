#include "Player.h"
#include "../Client/src/core/CoreDefinitions.h"

namespace TicTacToe
{
	Player::Player() : m_Name("DefaultName"), m_PieceShape(nullptr)
	{
		
	}

	Player::Player(std::string name, sf::Shape* pieceShape) : m_Name(std::move(name)), m_PieceShape(pieceShape)
	{

	}

	Player::~Player()
	{
		NULLPTR(m_PieceShape);
	}
}
