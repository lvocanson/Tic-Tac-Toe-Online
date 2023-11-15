#include "src/pch.h"
#include "FontRegistry.h"

std::map<std::string, sf::Font*> FontRegistry::m_Fonts;


sf::Font* FontRegistry::GetFont(const std::string& fontName)
{
    if (!m_Fonts.contains(fontName))
    {
        DebugLog("Font not registered: " + fontName);
    }

    return m_Fonts.at(fontName);
}

void FontRegistry::LoadFont(const std::string& fontName)
{
    auto* font = new sf::Font;

    if (!font->loadFromFile("resources/fonts/" + fontName + ".ttf"))
    {
        assert(false, "Failed to load font");
    }

    m_Fonts.insert(std::pair(fontName, font));
}

void FontRegistry::ClearFonts()
{
    for (auto& pair : m_Fonts)
    {
        RELEASE(pair.second)
    }

    m_Fonts.clear(); // Efface le contenu du map
}
