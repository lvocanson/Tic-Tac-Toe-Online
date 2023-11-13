#pragma once
#include <string>

#define EMPTY_PIECE (-1)

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
		PlayerData* GetPlayerData() { return &m_PlayerData; }

	private:

        PlayerData m_PlayerData;

	};

}
