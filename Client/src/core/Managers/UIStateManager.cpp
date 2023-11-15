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
    for (const auto& text : m_Texts)
    {
        delete text;
    }

    m_Texts.clear();
}

void UIStateManager::RegisterText(sf::Text* text)
{
    m_Window->RegisterDrawable(text);
    m_Texts.push_back(text);
}

void UIStateManager::UnregisterText(sf::Text* text)
{
    m_Window->UnregisterDrawable(text);
    std::erase(m_Texts, text);
    delete text;
}
