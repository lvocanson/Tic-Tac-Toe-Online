#pragma once
#include "IManager.h"
#include <src/core/Window.h>
#include "src/core/Managers/FontRegistry.h"


class UIStateManager : public IManager
{
public:

    UIStateManager(Window* window);
    ~UIStateManager() override;

    void Init() override;
    void Clear() override;

    void RegisterText(sf::Text* text);
    void UnregisterText(sf::Text* text);

protected:

    std::vector<sf::Text*> m_Texts;
    Window* m_Window;

};

