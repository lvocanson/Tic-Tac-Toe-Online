#pragma once
#include "UIStateManager.h"

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

    void UpdatePlayerTurnText(const PlayerData& data);
    void UpdateGameStateText(const std::string& text);
    void UpdatePlayerScore(const PlayerData& playerID, unsigned int score);

private:

    sf::Text* m_PlayerTurnText;
    sf::Text* m_GameStateText;
    sf::Text* m_Title;

    std::unordered_map<TicTacToe::PieceID, sf::Text*> m_PlayerScoreTexts;

};
