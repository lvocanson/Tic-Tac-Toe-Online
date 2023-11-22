#include "src/pch.h"
#include "FontRegistry.h"

std::unordered_map<std::string, sf::Font*> FontRegistry::m_Fonts;

sf::Font* FontRegistry::GetFont(const std::string& fontName)
{
    if (!m_Fonts.contains(fontName))
    {
        throw std::runtime_error("Font not registered: " + fontName);
    }

    return m_Fonts.at(fontName);
}

void FontRegistry::LoadFont(const std::string& fontName)
{
    auto* font = new sf::Font;

    try
    {
        if (!font->loadFromFile("resources/fonts/" + fontName))
        {
            throw std::runtime_error("Failed to load font: " + fontName);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;

        delete font;

        throw;
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
