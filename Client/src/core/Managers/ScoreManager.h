#pragma once
#include <map>
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

        void Init();

        void CreateScoreForPlayer(PlayerData* player);

        void AddPlayerMove(int playerID, int lastCellPlayed);
        void AddScoreToPlayer(int playerID);
        void InitNewGame(PlayerData* winner);

        int GetPlayerScore(int playerID);

    private:

        void ClearMoves();

    private:

        // Player id -> score
        std::map<int, int> m_PlayerScores;

        // List of all the local game history
        std::vector<GameData*> m_GameHistory;

        // List of all the moves of the current game
        std::vector<PlayerMove*> m_CurrentGame;
    };
}

