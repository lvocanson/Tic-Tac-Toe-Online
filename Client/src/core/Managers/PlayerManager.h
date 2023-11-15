#pragma once
#include "Player.h"
#include "IManager.h"
#include <map>

namespace TicTacToe {

    class PlayerManager : public IManager
    {
    public:

        PlayerManager();
        ~PlayerManager() override;

        void Init() override;
        void Clear() override;

        void SwitchPlayerTurn();

        void CreateNewPlayer(std::string name);
        void UnregisterPlayer(Player* player);

        // TODO: rework this
        bool IsPlayerOneTurn() { return m_IsPlayerOneTurn; }

        std::vector<Player*> GetAllPlayers() { return m_RegisteredPlayers; }
        Player* GetPlayer(int index) { return m_RegisteredPlayers[index]; }
        Player* GetCurrentPlayer() { return m_CurrentPlayer; }
        int GetPlayerCount() { return m_PlayerCount; }

    private:

        std::vector<Player*> m_RegisteredPlayers;
        Player* m_CurrentPlayer;

        int m_PlayerCount;
        int m_CurrentPlayerIndex;

        // TODO: rework this
        bool m_IsPlayerOneTurn;
    };
};
