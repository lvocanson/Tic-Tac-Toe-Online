#include "src/pch.h"
#include "GameStateUI.h"

#include "src/core/Managers/PlayerManager.h"

GameStateUI::GameStateUI(Window* window) : UIStateManager(window)
{
    m_PlayerScoreTexts = std::unordered_map<TicTacToe::PieceID, sf::Text*>();
    m_Title = nullptr;
    m_PlayerTurnText = nullptr;
    m_GameStateText = nullptr;
    m_ProgressBar = nullptr;
}

GameStateUI::~GameStateUI()
{
    Clear();
}

void GameStateUI::Init()
{
    UIStateManager::Init();

    const auto& font = *FontRegistry::GetFont("bold-font");
    const auto playerData = PlayerManager::GetCurrentPlayer()->GetData();

    m_Title = new sf::Text();
    m_Title->setFont(font);
    m_Title->setString("Tic Tac Toz");
    m_Title->setCharacterSize(48);
    m_Title->setFillColor(sf::Color::White);
    m_Title->setStyle(sf::Text::Bold | sf::Text::Underlined);
    m_Title->setPosition(m_Window->GetCenter().x - m_Title->getGlobalBounds().width * 0.5f + 20, 0.0f + 20);

    RegisterText(m_Title);

    m_PlayerTurnText = new sf::Text();
    m_PlayerTurnText->setFont(font);
    m_PlayerTurnText->setString(playerData->Name+ " turn");
    m_PlayerTurnText->setCharacterSize(24);
    m_PlayerTurnText->setFillColor(playerData->Color);
    m_PlayerTurnText->setStyle(sf::Text::Bold);
    m_PlayerTurnText->setPosition(m_Window->GetWidth() - m_PlayerTurnText->getGlobalBounds().width - 75, m_Window->GetHeight() * 0.5f - m_PlayerTurnText->getGlobalBounds().height);

    RegisterText(m_PlayerTurnText);

    m_GameStateText = new sf::Text();
    m_GameStateText->setFont(font);
    m_GameStateText->setString("");
    m_GameStateText->setCharacterSize(24);
    m_GameStateText->setFillColor(sf::Color::White);
    m_GameStateText->setStyle(sf::Text::Bold);
    m_GameStateText->setPosition(m_PlayerTurnText->getPosition().x, 100);

    RegisterText(m_GameStateText);
}

void GameStateUI::InitPlayerScores(const std::vector<Player*>& allPlayers)
{
    for (const auto player : allPlayers)
    {
        auto playerScoreText = new sf::Text();
        playerScoreText->setFont(*FontRegistry::GetFont("bold-font"));
        playerScoreText->setString(player->GetName() + " : 0");
        playerScoreText->setCharacterSize(24);
        playerScoreText->setFillColor(sf::Color::White);
        playerScoreText->setStyle(sf::Text::Bold);
        playerScoreText->setPosition(75, m_Window->GetHeight() * 0.5f - playerScoreText->getGlobalBounds().height + 25 * m_PlayerScoreTexts.size());

        RegisterText(playerScoreText);

        m_PlayerScoreTexts.insert(std::pair(player->GetPlayerID(), playerScoreText));
    }
}

void GameStateUI::InitProgressBar(const float maxValue)
{
    m_ProgressBar = new ProgressBar();
    m_ProgressBar->SetPosition(sf::Vector2f(m_PlayerTurnText->getPosition().x, m_PlayerTurnText->getPosition().y + 60));
    m_ProgressBar->SetSize(sf::Vector2f(200, 20));
    m_ProgressBar->SetMaxValue(100);
    m_ProgressBar->SetForegroundColor(PlayerManager::GetCurrentPlayer()->GetColor());

    m_Window->RegisterDrawable(m_ProgressBar);

    m_ProgressBar->SetMaxValue(maxValue);
    m_ProgressBar->SetValue(maxValue);
}

void GameStateUI::UpdatePlayerTurnText(const PlayerData& data)
{
    m_PlayerTurnText->setString(data.Name + " turn");
    m_PlayerTurnText->setFillColor(data.Color);

    if (m_ProgressBar)
        m_ProgressBar->SetForegroundColor(data.Color);
}

void GameStateUI::UpdateGameStateText(const std::string& text)
{
    m_GameStateText->setString(text);
}

void GameStateUI::UpdatePlayerScore(const PlayerData& player, const unsigned int score)
{
    if (!m_PlayerScoreTexts.contains(player.Id))
    {
        DebugLog("Player score text with " + std::to_string(player.Id) + " is not found !");
        return;
    }

    m_PlayerScoreTexts.at(player.Id)->setString(player.Name + " : " + std::to_string(score));
}

void GameStateUI::Clear()
{
    for (auto& text : m_PlayerScoreTexts)
    {
        NULLPTR(text.second);
    }

    NULLPTR(m_PlayerTurnText);
    NULLPTR(m_GameStateText);
    NULLPTR(m_Title);
}