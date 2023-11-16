#pragma once
#include <SFML/Graphics.hpp>

enum TextAlignment
{
    Left,
    Center,
    Right
};

class ButtonComponent;

class TextComponent : public sf::Drawable
{
public:
    TextComponent(const std::string& text, ButtonComponent& parentButton, const sf::Color& color, unsigned int size, TextAlignment alignment = TextAlignment::Left);

    void SetPosition(const sf::Vector2f& position);
    void SetPositionCentered(const sf::Vector2f& position);
    void SetTextAlignment(TextAlignment alignment);

private:
    sf::Text m_Text;
    TextAlignment m_Alignment;
    ButtonComponent& m_ParentButton;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
