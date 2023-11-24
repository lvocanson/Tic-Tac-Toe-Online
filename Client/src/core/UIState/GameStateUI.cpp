#include "src/pch.h"
#include "GameStateUI.h"

#include "src/core/Managers/PlayerManager.h"

GameStateUI::GameStateUI(Window* window) : UIStateManager(window)
{
    m_PlayerScoreTexts = std::unordered_map<TicTacToe::Piece, sf::Text*>();
    m_Title = nullptr;
    m_GameStateText = nullptr;
    m_GameModeText = nullptr;
    m_LobbyIDText = nullptr;
    m_UserName = nullptr;
    m_ProgressBar = nullptr;
}

GameStateUI::~GameStateUI()
{
    Clear();
}

void GameStateUI::Init()
{
    UIStateManager::Init();

    const auto& font = *FontRegistry::GetFont("coolvetica.otf");

    m_Title = new sf::Text();
    m_Title->setFont(font);
    m_Title->setString("Tic Tac Toz");
    m_Title->setCharacterSize(48);
    m_Title->setFillColor(sf::Color::White);
    m_Title->setStyle(sf::Text::Bold | sf::Text::Underlined);
    m_Title->setPosition(m_Window->GetCenter().x - m_Title->getGlobalBounds().width * 0.5f + 20, 0.0f + 20);

    RegisterText(m_Title);

    m_GameStateText = new sf::Text();
    m_GameStateText->setFont(font);
    m_GameStateText->setString("Waiting for another player...");
    m_GameStateText->setCharacterSize(38);
    m_GameStateText->setFillColor(sf::Color::White);
    m_GameStateText->setStyle(sf::Text::Bold);
    m_GameStateText->setPosition(m_Window->GetCenter().x - m_GameStateText->getGlobalBounds().width * 0.5f + 20, 0.0f + 70);

    RegisterText(m_GameStateText);
}

void GameStateUI::InitPlayerScores(const std::vector<Player*>& allPlayers)
{
    for (auto& text : m_PlayerScoreTexts)
    {
        m_Window->UnregisterDrawable(text.second);
        RELEASE(text.second);
    }

    int i = 1;
    for (const auto player : allPlayers)
    {
        auto playerScoreText = new sf::Text();
        playerScoreText->setFont(*FontRegistry::GetFont("coolvetica.otf"));
        playerScoreText->setString(player->GetName() + " : 0");
        playerScoreText->setCharacterSize(24);
        playerScoreText->setFillColor(PlayerShapeRegistry::GetPlayerColor(player->GetPiece()));
        playerScoreText->setStyle(sf::Text::Bold);
        playerScoreText->setPosition(55, m_Window->GetHeight() * 0.5f + i * 20);

        RegisterText(playerScoreText);

        m_PlayerScoreTexts.insert(std::pair(player->GetPiece(), playerScoreText));
        i++;
    }
}

void GameStateUI::InitProgressBar(const float maxValue)
{
    m_ProgressBar = new ProgressBar();
    m_ProgressBar->SetPosition(sf::Vector2f(m_GameStateText->getPosition().x, m_GameStateText->getPosition().y + 60));
    m_ProgressBar->SetSize(sf::Vector2f(200, 20));
    m_ProgressBar->SetMaxValue(100);
    m_ProgressBar->SetForegroundColor(PlayerManager::GetCurrentPlayer()->GetColor());

    m_Window->RegisterDrawable(m_ProgressBar);

    m_ProgressBar->SetMaxValue(maxValue);
    m_ProgressBar->SetValue(maxValue);
}

void GameStateUI::UpdatePlayerTurnText(const std::string& name, const sf::Color& color)
{
    m_GameStateText->setString(name + " turn");
    m_GameStateText->setFillColor(color);

    if (m_ProgressBar)
        m_ProgressBar->SetForegroundColor(color);
}

void GameStateUI::UpdateGameStateText(const std::string& text)
{
    m_GameStateText->setFillColor(sf::Color::White);
    m_GameStateText->setString(text);
}

void GameStateUI::UpdatePlayerScore(const TicTacToe::Piece piece, const std::string& name, const unsigned int score)
{
    if (!m_PlayerScoreTexts.contains(piece))
    {
        DebugLog("Player score text with is not found !");
        return;
    }

    m_PlayerScoreTexts.at(piece)->setString(name + " : " + std::to_string(score));
}

void GameStateUI::SetGameModeAndIDText(unsigned int& id, const std::string& gameMode)
{
    m_LobbyIDText = new sf::Text();
    m_LobbyIDText->setFont(*FontRegistry::GetFont("coolvetica.otf"));
    m_LobbyIDText->setString("LobbyID: " + std::to_string(id));
    m_LobbyIDText->setCharacterSize(24);
    m_LobbyIDText->setFillColor(sf::Color::White);
    m_LobbyIDText->setStyle(sf::Text::Bold);
    m_LobbyIDText->setPosition(55, 55);
   
    m_GameModeText = new sf::Text();
    m_GameModeText->setFont(*FontRegistry::GetFont("coolvetica.otf"));
    m_GameModeText->setString(gameMode);
    m_GameModeText->setCharacterSize(24);
    m_GameModeText->setFillColor(sf::Color::White);
    m_GameModeText->setStyle(sf::Text::Bold);
    m_GameModeText->setPosition(55, 80);
    
    RegisterText(m_GameModeText);
    RegisterText(m_LobbyIDText);
}

void GameStateUI::SetUserName(const std::string& username)
{
    m_UserName = new sf::Text();
    m_UserName->setFont(*FontRegistry::GetFont("coolvetica.otf"));
    m_UserName->setString("User: " + username);
    m_UserName->setCharacterSize(24);
    m_UserName->setFillColor(sf::Color::White);
    m_UserName->setStyle(sf::Text::Bold);
    m_UserName->setPosition(55, 105);

    RegisterText(m_UserName);
}

void GameStateUI::Clear()
{
    m_Window->ClearAllDrawables();

    for (auto& text : m_PlayerScoreTexts)
    {
        NULLPTR(text.second);
    }

    NULLPTR(m_GameStateText);
    NULLPTR(m_GameModeText);
    NULLPTR(m_LobbyIDText);
    NULLPTR(m_UserName);
    NULLPTR(m_Title);

    m_PlayerScoreTexts.clear();
}
