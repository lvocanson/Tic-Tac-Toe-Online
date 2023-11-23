#pragma once
#include "src/core/Managers/ScoreManager.h"
#include "game/GameData.h"

class GameHistoryManager : public IManager
{
public:
    GameHistoryManager();
    ~GameHistoryManager() override;

    void Init() override {};
    void Clear() override;

    void SaveGame(const std::vector<const PlayerMove*>* playerMoves);

    size_t GetGameHistorySize() const { return m_GameHistory.size(); }
    GameData* GetGameData(unsigned int gameID) const;
    std::vector<GameData*>& GetAllGameData() { return m_GameHistory; }

private:

    std::vector<GameData*> m_GameHistory;
};