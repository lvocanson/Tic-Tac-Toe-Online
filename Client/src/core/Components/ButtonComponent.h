#pragma once
#include "BaseComponent.h"
#include "TextComponent.h"

class ButtonComponent : public BaseComponent
{
public:
    ButtonComponent(const sf::Vector2f pos, const sf::Vector2f size, const sf::Color& idleColor);

    ~ButtonComponent() override;

    void Update(float dt) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void SetPosition(const sf::Vector2f& position) override;
    sf::Vector2f GetPosition() const override;
    sf::Vector2f GetSize() const override;

    void SetOnClickCallback(std::function<void()> onClickCallback);

    void SetButtonText(const std::string& text, const sf::Color& textColor, unsigned int textSize, TextAlignment textAlignment);

private:
    sf::RectangleShape m_Shape;
    sf::Color m_IdleColor;
    sf::Color m_HoverColor;
    sf::Color m_ClickedColor;

    std::function<void()> onClickCallback;

    TextComponent* m_Text = nullptr;;

    bool IsMouseOver();
};
