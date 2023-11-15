#pragma once

class FontRegistry
{
public:
    ~FontRegistry() = default;

    static sf::Font* GetFont(const std::string& fontName);
    static void LoadFont(const std::string& fontName);

    static void ClearFonts();

private:
    FontRegistry() = delete;

    static std::map<std::string, sf::Font*> m_Fonts;

};

