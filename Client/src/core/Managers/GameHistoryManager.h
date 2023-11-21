#pragma once
#include "src/core/Managers/ScoreManager.h"

class GameHistoryManager : public IManager
{
public:
	GameHistoryManager();
	~GameHistoryManager() override;

	void Init() override;
	void Clear() override;

	void SaveGame(const PlayerData* winner);
	std::vector<GameData*>& GetGameHistory() { return m_GameHistory; }

private:

	void ClearMoves();
private:

	std::vector<GameData*> m_GameHistory;
	std::vector<PlayerMove> m_CurrentGame;
};