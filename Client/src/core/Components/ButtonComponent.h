#pragma once
#include "src/core/Window.h"
#include <functional>

class ButtonComponent : public sf::Drawable
{
public:
    ButtonComponent(float x, float y, float width, float height, const sf::Color& idleColor, const sf::Color& hoverColor);

    void Update(Window* m_Window);
    bool IsMouseOver(Window* m_Window);
    void SetOnClickCallback(std::function<void()> onClickCallback);

private:
    sf::RectangleShape shape;
    sf::Color idleColor;
    sf::Color hoverColor;

    Window* m_Window;
    std::function<void()> onClickCallback;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
