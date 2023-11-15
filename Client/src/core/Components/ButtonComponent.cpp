#include "ButtonComponent.h"

ButtonComponent::ButtonComponent(float x, float y, float width, float height, const sf::Color& idleColor, const sf::Color& hoverColor)
	: idleColor(idleColor)
	, hoverColor(hoverColor)
{
	shape.setPosition(sf::Vector2f(x, y));
	shape.setSize(sf::Vector2f(width, height));
	shape.setFillColor(idleColor);
}

void ButtonComponent::Update(Window* m_Window)
{
	if (IsMouseOver(m_Window))
	{
		shape.setFillColor(hoverColor);
	}
	else
	{
		shape.setFillColor(idleColor);
	}

	if (IsMouseOver(m_Window) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (onClickCallback) {
			onClickCallback();
		}
	}
}

bool ButtonComponent::IsMouseOver(Window* m_Window)
{
	sf::Vector2f mousePos = (sf::Vector2f)m_Window->GetMousePosition();
	sf::Vector2f shapePos = shape.getPosition();
	sf::Vector2f shapeSize = shape.getSize();

	if (mousePos.x > shapePos.x && mousePos.x < shapePos.x + shapeSize.x
		&& mousePos.y > shapePos.y && mousePos.y < shapePos.y + shapeSize.y)
	{
		return true;
	}
	return false;
}

void ButtonComponent::SetOnClickCallback(std::function<void()> onClickCallback) {
    this->onClickCallback = onClickCallback;
}

void ButtonComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape, states);
}
