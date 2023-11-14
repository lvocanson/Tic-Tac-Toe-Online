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

        void RegisterNewPlayer(std::string name);
        void UnregisterPlayer(Player* player);

        int GetPlayerCount() { return m_PlayerCount; }

    private:

        std::vector<Player*> m_RegisteredPlayers;
        Player* m_CurrentPlayer;

        int m_PlayerCount;
        int m_CurrentPlayerIndex;

    };
};
