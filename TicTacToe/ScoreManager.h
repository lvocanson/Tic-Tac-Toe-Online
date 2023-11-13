#pragma once
#include <vector>

#include "Player.h"

namespace TicTacToe
{
    class PlayerMove
    {
    public:

        PlayerMove(int playerID, int boardCell);

    private:

        int PlayerID;
        int BoardCell;
    };

    class GameData
    {
    public:

        GameData(PlayerData* winner, const std::vector<PlayerMove*>& allMoves);
        ~GameData();

    private:

        PlayerData* Winner;
        std::vector<PlayerMove*> AllMoves;
    };

    class ScoreManager
    {
    public:

        ScoreManager();
        ~ScoreManager();

        void AddMove(int playerID, int lastCellPlayed);
        
        void NewGame(PlayerData* winner);

    private:

        void ClearMoves();

    private:

        std::vector<GameData*> m_GameHistory;
        std::vector<PlayerMove*> m_CurrentGame;
    };
}

