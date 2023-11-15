#pragma once
#include <map>
#include <vector>

#include "src/core/Window.h"
#include "src/core/Player.h"
#include "IManager.h"


namespace TicTacToe
{
    struct PlayerMove
    {
        PieceID PieceID;
        unsigned int BoardCell;
    };

    class GameData
    {
    public:

        GameData(const PlayerData* winner, const std::vector<PlayerMove>& allMoves);
        ~GameData();

    private:

        const PlayerData* Winner;
        std::vector<PlayerMove> AllMoves;
    };

    class ScoreManager : public IManager
    {
    public:

        ScoreManager();
        ~ScoreManager() override;

        void Init() override;
        void Clear() override;

        void CreateScoreForPlayer(PlayerData* playerData, Window* window);

        void AddPlayerMove(PieceID pieceID, unsigned int lastCellPlayed);
        void AddScoreToPlayer(const PlayerData* player);
        void SaveGame(const PlayerData* winner);

        unsigned int GetPlayerScore(PieceID pieceID);

    private:

        void ClearMoves();

    private:

        // Player id -> score
        std::map<PieceID, unsigned int> m_PlayerScores;

        // List of all the local game history
        std::vector<GameData*> m_GameHistory;

        // List of all the moves of the current game
        std::vector<PlayerMove> m_CurrentGame;

    };
}

