#pragma once
#include "src/core/Managers/ScoreManager.h"

class GameData
{
public:

    GameData(const PlayerData* winner, const std::vector<const PlayerMove*>* allMoves);
    ~GameData();

    const std::vector<const PlayerMove*>* GetMoves() const { return AllMoves; }
    const PlayerMove* GetMove(unsigned int moveIndex) const { return AllMoves->at(moveIndex); }
    size_t GetMovesSize() const { return AllMoves->size(); }

private:

    const PlayerData* Winner;
    const std::vector<const PlayerMove*>* AllMoves;
};

class GameHistoryManager : public IManager
{
public:
    GameHistoryManager();
    ~GameHistoryManager() override;

    void Init() override;
    void Clear() override;

    void SaveGame(const PlayerData* winner, const std::vector<const PlayerMove*>* playerMoves);

    size_t GetGameHistorySize() const { return m_GameHistory.size(); }
    GameData* GetGameData(unsigned int gameID);
    std::vector<GameData*>& GetAllGameData() { return m_GameHistory; }

private:

    std::vector<GameData*> m_GameHistory;
};