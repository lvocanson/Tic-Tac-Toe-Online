#pragma once
#include <map>
#include <vector>

#include "Player.h"
#include "IManager.h"
#include <src/core/Window.h>

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

    class ScoreManager : public IManager
    {
    public:

        ScoreManager();
        ~ScoreManager() override;

        void Init() override;
        void Clear() override;

        void CreateScoreForPlayer(PlayerData* player, Window* window);

        void AddPlayerMove(int playerID, int lastCellPlayed);
        void AddScoreToPlayer(PlayerData* player);
        void SaveGame(PlayerData* winner);

        int GetPlayerScore(int playerID);

    private:

        void ClearMoves();

    private:

        // Player id -> score
        std::map<int, int> m_PlayerScores;
        std::map<int, sf::Text*> m_PlayerScoreTexts;

        // List of all the local game history
        std::vector<GameData*> m_GameHistory;

        // List of all the moves of the current game
        std::vector<PlayerMove*> m_CurrentGame;


        sf::Font m_Font;
    };
}

