#pragma once
#include "BaseComponent.h"
#include "TextComponent.h"

class ProgressBar : public BaseComponent
{
public:

    ProgressBar();
    ProgressBar(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& backgroundColor, const sf::Color& foregroundColor, const sf::Font& font, float maxValue);
    ~ProgressBar() override;

    void Update() override {}

    void SetSize(const sf::Vector2f& size);
    void SetPosition(const sf::Vector2f& position) override;
    sf::Vector2f GetPosition() const override;
    sf::Vector2f GetSize() const override;

    void SetBackgroundColor(const sf::Color& fillColor) { m_Background.setFillColor(fillColor);}
    void SetBackgroundOutlineColor(const sf::Color& outlineColor) { m_Background.setOutlineColor(outlineColor); }
    void SetBackgroundOutlineThickness(float thickness) { m_Background.setOutlineThickness(thickness); }

    void SetForegroundColor(const sf::Color& fillColor) { m_Foreground.setFillColor(fillColor); }
    void SetForegroundOutlineColor(const sf::Color& outlineColor) { m_Foreground.setOutlineColor(outlineColor); }
    void SetForegroundOutlineThickness(float thickness) { m_Foreground.setOutlineThickness(thickness); }

    void SetMaxValue(float maxValue);
    void SetValue(float currentValue);

private:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::RectangleShape m_Background;
    sf::RectangleShape m_Foreground;
    //TextComponent m_Text;

    float m_MaxValue;
    float m_CurrentValue;
};

