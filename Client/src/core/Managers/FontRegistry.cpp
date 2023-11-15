#include "src/pch.h"
#include "FontRegistry.h"

FontRegistry::FontRegistry()
{
    m_Fonts = std::map<std::string, sf::Font>();
}

sf::Font FontRegistry::GetFont(std::string fontName)
{
    if (m_Fonts.find(fontName) == m_Fonts.end())
    {
        std::cout << "Font not found: " << fontName << std::endl;
    }

    return m_Fonts[fontName];
}

void FontRegistry::RegisterFont(const std::string& fontName)
{
    sf::Font font;

    if (!font.loadFromFile("resources/fonts/" + fontName + ".ttf"))
    {
        assert(false, "Failed to load font");
    }

    m_Fonts[fontName] = font;
}
