#pragma once

class FontRegistry
{
public:
    FontRegistry();
    ~FontRegistry() = default;

    static sf::Font GetFont(std::string fontName);

    static void RegisterFont(const std::string& fontName);

private:

    static std::map<std::string, sf::Font> m_Fonts;

};

