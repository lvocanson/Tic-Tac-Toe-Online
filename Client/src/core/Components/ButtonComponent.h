#pragma once
#include "src/core/Window.h"
#include <functional>
#include "TextComponent.h"

class ButtonComponent : public sf::Drawable
{
public:
    ButtonComponent(const sf::Vector2f pos, const sf::Vector2f size, const sf::Color& idleColor, const sf::Color& hoverColor, const std::string& buttonText, const sf::Color& textColor, unsigned int textSize, TextAlignment textAlignment);
    ~ButtonComponent();

    void Update(Window* m_Window);
    bool IsMouseOver(Window* m_Window);
    void SetOnClickCallback(std::function<void()> onClickCallback);
    sf::Vector2f GetPosition() const;
    sf::Vector2f GetSize() const;

private:
    sf::RectangleShape shape;
    sf::Color idleColor;
    sf::Color hoverColor;

    Window* m_Window;
    std::function<void()> onClickCallback;

    TextComponent m_Text;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
