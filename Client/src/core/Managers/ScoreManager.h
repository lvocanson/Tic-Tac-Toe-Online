#pragma once
#include <map>
#include <vector>

#include "src/core/Window.h"
#include "src/core/Player.h"
#include "IManager.h"



struct PlayerMove
{
    PlayerData playerData;
    unsigned int BoardCell;
};

class GameData
{
public:

    GameData(const PlayerData* winner, const std::vector<PlayerMove>& allMoves);
    ~GameData();

    const std::vector<PlayerMove>& GetAllMoves() const { return AllMoves; }

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

    void AddPlayerMove(PlayerData playerData, unsigned int lastCellPlayed);
    void AddScoreToPlayer(TicTacToe::PieceID player);

    unsigned int GetPlayerScore(TicTacToe::PieceID pieceID);

    bool IsScoreExists(TicTacToe::PieceID& pieceID);

    const std::vector<PlayerMove>& GetCurrentGame() const { return m_CurrentGame; }

private:

    // Player id -> score
    std::map<TicTacToe::PieceID, unsigned int> m_PlayerScores;

    std::vector<PlayerMove> m_CurrentGame;
};

