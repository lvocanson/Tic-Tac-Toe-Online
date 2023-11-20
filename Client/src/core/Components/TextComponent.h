#pragma once
#include "BaseComponent.h"

enum TextAlignment
{
    Left,
    Center,
    Right
};

class TextComponent : public BaseComponent
{
public:
    TextComponent();
    TextComponent(const std::string& text, BaseComponent* parentComponent, const sf::Color& color, unsigned int size, TextAlignment alignment);

    void Update() override;
    void SetPosition(const sf::Vector2f& position) override;
    void SetTextAlignment(TextAlignment alignment);
    void SetPositionCentered(const sf::Vector2f& position);
    void SetText(const std::string& text);
    void SetColor(const sf::Color& color);
    void SetCharacterSize(unsigned int size);

    sf::Vector2f GetPosition() const override;
    sf::Vector2f GetSize() const override { return sf::Vector2f(m_Text.getLocalBounds().width, m_Text.getLocalBounds().height); }


private:
    sf::Text m_Text;
    TextAlignment m_Alignment;

    BaseComponent* m_ParentComponent;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
