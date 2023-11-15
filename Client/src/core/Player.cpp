#include "Player.h"
#include "IDGenerator.h"


Player::Player() : m_Name("DefaultName")
{
	m_ID = IDGenerator::GenerateID();
}

Player::Player(std::string name) : m_Name(std::move(name))
{
	m_ID = IDGenerator::GenerateID();
}

Player::~Player()
{
}
