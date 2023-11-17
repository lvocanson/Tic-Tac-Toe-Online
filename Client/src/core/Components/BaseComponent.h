#pragma once

class BaseComponent : public sf::Drawable
{
public:
    virtual void Update() = 0;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

    virtual void SetPosition(const sf::Vector2f& position) = 0;
    virtual sf::Vector2f GetPosition() const = 0;
    virtual sf::Vector2f GetSize() const = 0;

    virtual ~BaseComponent() {}
};