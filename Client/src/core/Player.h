#pragma once
#include <string>

struct PlayerData
{
    std::string Name;
    TicTacToe::PieceID Id;
	sf::Color Color;
};

class Player
{
public:

	Player();
	Player(const std::string& name, const sf::Color color);
	~Player();

	void SetColor(const sf::Color color) { m_Data.Color = color; }

	sf::Color GetColor() const { return m_Data.Color; }
	TicTacToe::PieceID GetPlayerID() const { return m_Data.Id; }
	std::string GetName() const { return m_Data.Name; }
	PlayerData* GetData() { return &m_Data; }

private:

	PlayerData m_Data;

};
