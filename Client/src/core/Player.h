#pragma once
#include <string>
#include "TicTacToe.h"

class Player
{
public:

	Player();

	/// <summary>
	/// Init a player with a name
	/// </summary>
	Player(std::string name);
	~Player();

	void SetName(const std::string& name) { m_Name = name; }

	TicTacToe::Piece GetPlayerID() const { return m_ID; }
	std::string GetName() const { return m_Name; }

private:

	std::string m_Name;
	TicTacToe::Piece m_ID;

};