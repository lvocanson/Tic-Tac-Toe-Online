#include "ScoreManager.h"
#include <cassert>

TicTacToe::PlayerMove::PlayerMove(int playerID, int boardCell)
{
    PlayerID = playerID;
    BoardCell = boardCell;
}

TicTacToe::GameData::GameData(PlayerData* winner, const std::vector<PlayerMove*>& allMoves)
{
    Winner = winner;
    AllMoves = allMoves;
}

TicTacToe::GameData::~GameData()
{
    for (auto move : AllMoves)
    {
        RELEASE(move)
    }

    AllMoves.clear();
}

TicTacToe::ScoreManager::ScoreManager()
{
    m_GameHistory = std::vector<GameData*>();
    m_CurrentGame = std::vector<PlayerMove*>();
    m_PlayerScores = std::map<int, int>();
}

TicTacToe::ScoreManager::~ScoreManager()
{
    Clear();
}

void TicTacToe::ScoreManager::Init()
{
    if (!m_Font.loadFromFile("resources/fonts/bold-font.ttf"))
    {
        assert(false, "Failed to load font");
    }
}

void TicTacToe::ScoreManager::Clear()
{
for (auto game : m_GameHistory)
    {
        RELEASE(game)
    }

    for (auto move : m_CurrentGame)
    {
        RELEASE(move)
    }

    for (auto text : m_PlayerScoreTexts)
    {
        NULLPTR(text.second);
    }

    m_PlayerScores.clear();
    m_CurrentGame.clear();
    m_GameHistory.clear();
}

void TicTacToe::ScoreManager::CreateScoreForPlayer(PlayerData* player, Window* window)
{
    m_PlayerScores.insert(std::pair<int, int>(player->PlayerID, 0));

    auto* scoreText = new sf::Text();
    scoreText->setFont(m_Font);
    scoreText->setString(player->Name + " : ");
    scoreText->setCharacterSize(24);
    scoreText->setFillColor(sf::Color::White);
    scoreText->setStyle(sf::Text::Bold);
    scoreText->setPosition(75, window->GetHeight() * 0.5f - scoreText->getGlobalBounds().height + 25 * m_PlayerScores.size());

    window->RegisterDrawable(scoreText);

    m_PlayerScoreTexts.insert(std::pair<int, sf::Text*>(player->PlayerID, scoreText));
}

void TicTacToe::ScoreManager::AddPlayerMove(int playerID, int lastCellPlayed)
{
    m_CurrentGame.push_back(new PlayerMove(playerID, lastCellPlayed));
}

void TicTacToe::ScoreManager::AddScoreToPlayer(PlayerData* player)
{
    unsigned int id = player->PlayerID;

    m_PlayerScores[id]++;
    m_PlayerScoreTexts[id]->setString(player->Name + " : " + std::to_string(m_PlayerScores[id]));
}

int TicTacToe::ScoreManager::GetPlayerScore(int playerID)
{
    return m_PlayerScores[playerID];
}

void TicTacToe::ScoreManager::SaveGame(PlayerData* winner)
{
    m_GameHistory.push_back(new GameData(winner, m_CurrentGame));
    ClearMoves();
}

void TicTacToe::ScoreManager::ClearMoves()
{
    m_CurrentGame.clear();
}
