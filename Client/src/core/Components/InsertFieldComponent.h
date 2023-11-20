#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>
#include "src/core/Components/TextComponent.h"

class InsertFieldComponent : public BaseComponent
{
public:

    InsertFieldComponent();
    InsertFieldComponent(const sf::Vector2f& pos, const sf::Vector2f& size,
        const sf::Color& idleColor, const sf::Color& hoverColor,
        float outlineThickness, unsigned int characterLimit);

    ~InsertFieldComponent() override;
    void BlinkCursor(float dt);

    void Update(float dt) override;

    void SetLabel(const std::string& label) { m_Label.SetText(label); }
    void SetPosition(const sf::Vector2f& position) override;
    void SetSize(const sf::Vector2f& size) { m_Rectangle.setSize(size); }
    void SetFillColor(const sf::Color& color) { m_Rectangle.setFillColor(color); };
    void SetHoverColor(const sf::Color& color) { m_Rectangle.setOutlineColor(color); };
    void SetOutlineThickness(float thickness) { m_Rectangle.setOutlineThickness(thickness); };
    void SetCharacterLimit(unsigned int limit) { if (m_CharacterLimit > 0) m_CharacterLimit = limit; }

    sf::Vector2f GetPosition() const override { return m_Rectangle.getPosition(); }
    sf::Vector2f GetSize() const override { return m_Rectangle.getSize(); }
    bool GetFocus() const { return m_Focus; }
    std::string GetText() const { return m_TextStream.str(); }
    unsigned int GetCharacterLimit() const { return m_CharacterLimit; }
    unsigned int GetTextSize() const { return m_TextStream.str().length(); }

private:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    bool IsMouseOver();
    void ReplaceCursor();
    void AppendCharacter(const char& c);

private:

    sf::RectangleShape m_Rectangle;
    TextComponent m_Text;
    TextComponent m_Label;
    TextComponent m_Cursor;
    std::ostringstream m_TextStream;

    unsigned int m_CharacterLimit;
    bool m_Focus;
    float m_CursorTimer;
    const float m_BlinkTime = 0.5f;
};
