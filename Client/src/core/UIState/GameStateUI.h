#pragma once
#include "src/core/Components/ProgressBar.h"
#include "src/core/Managers/UIStateManager.h"

struct PlayerData;
class Player;

class GameStateUI : public UIStateManager
{
public:

    GameStateUI(Window* window);
    ~GameStateUI() override;

    void Init() override;
    void Clear() override;

    void InitPlayerScores(const std::vector<Player*>& allPlayers);
    void InitProgressBar(const float maxValue);

    void UpdatePlayerTurnText(const PlayerData& data);
    void UpdateGameStateText(const std::string& text);
    void UpdatePlayerScore(const PlayerData& playerID, unsigned int score);
    void UpdateProgressBar(float value) const { m_ProgressBar->SetValue(value); }
    void SetLobbyIDText(unsigned int& id);
    void SetUserName(const std::string username);

private:

    sf::Text* m_GameStateText;
    sf::Text* m_LobbyIDText;
    sf::Text* m_UserName;
    sf::Text* m_Title;
    ProgressBar* m_ProgressBar;

    std::unordered_map<TicTacToe::PieceID, sf::Text*> m_PlayerScoreTexts;
};
