#pragma once
#include <string>

struct PlayerData
{
    std::string Name;
    TicTacToe::PieceID Id;
};

class Player
{
public:

	Player();

	/// <summary>
	/// Init a player with a name
	/// </summary>
	Player(const std::string& name);
	~Player();

	TicTacToe::PieceID GetPlayerID() const { return m_Data.Id; }
	std::string GetName() const { return m_Data.Name; }
	PlayerData* GetData() { return &m_Data; }

private:

	PlayerData m_Data;

};