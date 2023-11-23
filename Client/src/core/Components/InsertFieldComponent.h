#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>
#include "src/core/Components/TextComponent.h"

constexpr unsigned int DEFAULT_CHARACTER_LIMIT = 20;
constexpr float CURSOR_BLINK_TIME = 0.5f;

class InsertFieldComponent : public BaseComponent
{
public:

    InsertFieldComponent();
    InsertFieldComponent(const sf::Vector2f& pos, const sf::Vector2f& size,
        const sf::Color& idleColor, const sf::Color& hoverColor,
        float outlineThickness, unsigned int characterLimit);

    ~InsertFieldComponent() override = default;

    void Update(float dt) override;

    void SetText(const std::string& text);
    void SetLabel(const std::string& label) { m_Label.SetText(label); }
    void SetPosition(const sf::Vector2f& position) override;
    void SetSize(const sf::Vector2f& size) { m_Rectangle.setSize(size); }
    void SetFillColor(const sf::Color& color) { m_Rectangle.setFillColor(color); }
    void SetHoverColor(const sf::Color& color) { m_Rectangle.setOutlineColor(color); }
    void SetOutlineThickness(float thickness) { m_Rectangle.setOutlineThickness(thickness); }
    void SetCharacterLimit(unsigned int limit) { if (limit > 0) m_CharacterLimit = limit; }
    void ShowErrorMessage(const std::string& message) { m_ErrorText.SetText(message); }
    void ClearErrorMessage() { m_ErrorText.SetText(""); }

    sf::Vector2f GetPosition() const override { return m_Rectangle.getPosition(); }
    sf::Vector2f GetSize() const override { return m_Rectangle.getSize(); }
    bool GetFocus() const { return m_Focus; }
    std::string GetText() const { return m_TextContent; }
    unsigned int GetCharacterLimit() const { return m_CharacterLimit; }
    size_t GetTextSize() const { return m_TextContent.length(); }

private:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    bool IsMouseOver();
    void ReplaceCursor();
    void AppendCharacter(const char& c);
    void BlinkCursor(float dt);

private:

    sf::RectangleShape m_Rectangle;
    TextComponent m_Text;
    TextComponent m_Label;
    TextComponent m_Cursor;
    TextComponent m_ErrorText;
    std::string m_TextContent;

    unsigned int m_CharacterLimit;
    bool m_Focus;
    float m_CursorTimer;
};
