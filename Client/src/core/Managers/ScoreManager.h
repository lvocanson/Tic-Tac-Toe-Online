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

class ScoreManager : public IManager
{
public:

    ScoreManager();
    ~ScoreManager() override;

    void Init() override;
    void Clear() override;

    void InitPlayerScores(const std::vector<Player*>& allPlayers);

    void AddPlayerMove(const PlayerData& playerData, unsigned int lastCellPlayed);
    void AddScoreToPlayer(TicTacToe::PieceID player);

    unsigned int GetPlayerScore(TicTacToe::PieceID pieceID);
    const std::vector<const PlayerMove*>* GetCurrentGame() const { return m_CurrentGame; }

    bool IsScoreExists(TicTacToe::PieceID& pieceID);
    void CreateNewGameHistory();
    void ResetCurrentGame();

private:

    // Player id -> score
    std::map<TicTacToe::PieceID, unsigned int> m_PlayerScores;

    std::vector<const PlayerMove*>* m_CurrentGame;
};

