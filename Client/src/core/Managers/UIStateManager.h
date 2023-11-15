#pragma once
#include "IManager.h"
#include <src/core/Window.h>

enum AvaiblableFonts
{
    Bold,
};

class UIStateManager : public IManager
{
public:

    UIStateManager(Window* window);
    virtual ~UIStateManager();

    void Init() override;
    void Clear() override;


protected:

    std::vector<sf::Text*> m_Texts;
    Window* m_Window;

};

