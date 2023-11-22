#include "ScoreManager.h"

ScoreManager::ScoreManager()
{
	m_PlayerScores = std::map<TicTacToe::PieceID, unsigned int>();
	m_CurrentGame = nullptr;
}

ScoreManager::~ScoreManager()
{
	Clear();
}

void ScoreManager::Init()
{
	m_CurrentGame = new std::vector<const PlayerMove*>();
}

void ScoreManager::Clear()
{
	m_PlayerScores.clear();

	if (m_CurrentGame == nullptr) return;

	for (auto move : *m_CurrentGame)
	{
		RELEASE(move)
	}
	RELEASE(m_CurrentGame)
}

void ScoreManager::InitPlayerScores(const std::vector<Player*>& allPlayers)
{
	for (auto& player : allPlayers)
	{
		m_PlayerScores.insert(std::pair<TicTacToe::PieceID, int>(player->GetPlayerID(), 0));
	}
}

void ScoreManager::AddPlayerMove(const PlayerData& playerData, unsigned int lastCellPlayed)
{
	const auto playerMove = new PlayerMove{
		.playerData = playerData,
		.BoardCell = lastCellPlayed
	};

	m_CurrentGame->push_back(playerMove);
}

void ScoreManager::AddScoreToPlayer(TicTacToe::PieceID pieceID)
{
	if (!IsScoreExists(pieceID)) return;

	m_PlayerScores[pieceID]++;
}

unsigned int ScoreManager::GetPlayerScore(TicTacToe::PieceID pieceID)
{
	if (!IsScoreExists(pieceID)) return 0;

	return m_PlayerScores[pieceID];
}

bool ScoreManager::IsScoreExists(TicTacToe::PieceID& pieceID)
{
	if (!m_PlayerScores.contains(pieceID))
	{
		DebugLog("Score isn't created for this player");
		return false;
	}

	return true;
}

void ScoreManager::CreateNewGameHistory()
{
	m_CurrentGame = new std::vector<const PlayerMove*>();
}

void ScoreManager::ResetCurrentGame()
{
	for (auto move : *m_CurrentGame)
	{
		RELEASE(move)
	}
	m_CurrentGame->clear();
}

