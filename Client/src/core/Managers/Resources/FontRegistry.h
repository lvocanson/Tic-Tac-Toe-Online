#pragma once

class FontRegistry
{
public:

    static sf::Font* GetFont(const std::string& fontName);
    static void LoadFont(const std::string& fontName);

    static void ClearFonts();

private:

    FontRegistry() = delete;
    ~FontRegistry() = delete;

    static std::unordered_map<std::string, sf::Font*> m_Fonts;

};

