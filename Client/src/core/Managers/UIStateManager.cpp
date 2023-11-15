#include "src/pch.h"
#include "UIStateManager.h"

UIStateManager::UIStateManager(Window* window) : m_Window(window)
{
    Init();
}

UIStateManager::~UIStateManager()
{
    Clear();
}

void UIStateManager::Init()
{
    
}

void UIStateManager::Clear()
{
    for (auto& text : m_Texts)
    {
        delete text;
    }

    m_Texts.clear();
}
