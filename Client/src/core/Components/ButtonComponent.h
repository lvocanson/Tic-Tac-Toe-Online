// ButtonComponent.h
#pragma once
#include "BaseComponent.h"
#include "TextComponent.h"
#include "src/core/Managers/InputHandler.h"
#include <memory>

class ButtonComponent : public BaseComponent, public sf::Transformable
{
public:
    ButtonComponent(const sf::Vector2f pos, const sf::Vector2f size, const sf::Color& idleColor, const sf::Color& hoverColor);

    ~ButtonComponent() override;

    void Update() override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void SetPosition(const sf::Vector2f& position) override;
    sf::Vector2f GetPosition() const override;
    sf::Vector2f GetSize() const override;

    void SetOnClickCallback(std::function<void()> onClickCallback);

    void SetButtonText(const std::string& text, const sf::Color& textColor, unsigned int textSize, TextAlignment textAlignment);

private:
    sf::RectangleShape shape;
    sf::Color idleColor;
    sf::Color hoverColor;
    std::function<void()> onClickCallback;

    TextComponent* m_Text;

    bool IsMouseOver();
};
