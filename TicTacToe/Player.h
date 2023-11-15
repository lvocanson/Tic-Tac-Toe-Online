#pragma once
#include <string>

#define EMPTY_PIECE (-1)
#define DEFAULT_PIECE_SIZE (150.f)

namespace TicTacToe
{
	
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

		int GetPlayerID() const { return m_ID; }
		std::string GetName() const { return m_Name; }

	private:

		std::string m_Name;
		int m_ID;

	};

}
