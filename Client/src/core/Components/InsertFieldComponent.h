#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>
#include <functional>
#include "src/core/Components/TextComponent.h"

class InsertFieldComponent : public sf::Drawable
{
public:
    InsertFieldComponent(const sf::Vector2f& pos, const sf::Vector2f& size,
        const sf::Color& idleColor, const sf::Color& hoverColor,
        float outlineThickness);

    ~InsertFieldComponent();

    void Update();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    bool IsMouseOver();

    void SetPosition(const sf::Vector2f& position);

    sf::Vector2f GetPosition() const { return m_Rectangle.getPosition(); }

    sf::Vector2f GetSize() const { return m_Rectangle.getSize(); }

    void SetFocus(bool focus);

    bool GetFocus() const { return m_Focus; }

    const std::string& GetText() { return m_TextStream.str(); }

private:
    sf::RectangleShape m_Rectangle;
    TextComponent m_Text;
    std::ostringstream m_TextStream;
    bool m_Focus;
};
