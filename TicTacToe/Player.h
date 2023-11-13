#pragma once
#include <string>

#define EMPTY_PIECE (-1)
#define DEFAULT_PIECE_SIZE (150)

namespace TicTacToe
{
	struct PlayerData
	{
	    int PlayerID;
        std::string Name;
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

		void SetName(const std::string& name) { m_PlayerData.Name = name; }

		int GetPlayerID() const { return m_PlayerData.PlayerID; }
		std::string GetName() const { return m_PlayerData.Name; }

	private:

        PlayerData m_PlayerData;

	};

}
