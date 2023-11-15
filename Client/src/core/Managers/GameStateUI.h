#pragma once
#include "UIStateManager.h"

class GameStateUI : public UIStateManager
{
public:

    GameStateUI(Window* window);
    ~GameStateUI() override;


private:

    sf::Text* m_PlayerTurnText;
    
};

