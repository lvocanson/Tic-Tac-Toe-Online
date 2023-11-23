#pragma once
#include <map>
#include <vector>

#include "src/core/Window.h"
#include "src/core/Player.h"
#include "IManager.h"

class ScoreManager : public IManager
{
public:

    ScoreManager();
    ~ScoreManager() override;

    void Init() override {};
    void Clear() override;

    void InitPlayerScores(const std::vector<Player*>& allPlayers);

    void AddScoreToPlayer(TicTacToe::Piece);

    unsigned int GetPlayerScore(TicTacToe::Piece);

    bool IsScoreExists(TicTacToe::Piece&);

private:

    // Player id -> score
    std::map<TicTacToe::Piece, unsigned int> m_PlayerScores;
};
