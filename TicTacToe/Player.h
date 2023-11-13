#pragma once
#include <string>

#include "../Client/vendor/SFML-2.6.1/include/SFML/Graphics/Shape.hpp"


namespace TicTacToe
{
	
	class Player
	{
	public:

		Player();

		/// <summary>
		/// Init a player with a name and a piece shape
		/// </summary>
		Player(std::string name, sf::Shape* pieceShape);
		~Player();

		void SetName(const std::string& name) { m_Name = name; }
		void SetShape(sf::Shape* shape) { m_PieceShape = shape; }

		int GetPlayerID() const { return m_PlayerID; }
		std::string GetName() const { return m_Name; }
		sf::Shape* GetShape() const { return m_PieceShape; }

	private:

		std::string m_Name;
		sf::Shape* m_PieceShape;
		int m_PlayerID;

	};

}
