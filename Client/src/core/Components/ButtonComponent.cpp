#include "ButtonComponent.h"

ButtonComponent::ButtonComponent(const sf::Vector2f pos, const sf::Vector2f size, const sf::Color& idleColor, const sf::Color& hoverColor, const std::string& buttonText, const sf::Color& textColor, unsigned int textSize, TextAlignment textAlignment)
    : idleColor(idleColor)
    , hoverColor(hoverColor)
    , onClickCallback(nullptr)
    , m_Window(nullptr)
    , m_Text(buttonText, *this, textColor, textSize, textAlignment)
{
    shape.setPosition(pos);
    shape.setSize(size);
    shape.setFillColor(idleColor);

    m_Text.SetPosition(pos);
}

ButtonComponent::ButtonComponent(const sf::Vector2f pos, const sf::Vector2f size, const sf::Color& idleColor, const sf::Color& hoverColor, const sf::Color& textColor, unsigned int textSize, TextAlignment textAlignment)
    : idleColor(idleColor)
    , hoverColor(hoverColor)
    , onClickCallback(nullptr)
    , m_Window(nullptr)
    , m_Text(*this, textColor, textSize, textAlignment)
{
	shape.setPosition(pos);
	shape.setSize(size);
	shape.setFillColor(idleColor);

	m_Text.SetPosition(pos);
}

ButtonComponent::~ButtonComponent()
{
}

void ButtonComponent::Update(Window* m_Window)
{
    if (IsMouseOver(m_Window)) {
        shape.setFillColor(hoverColor);
    }
    else {
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
	sf::Vector2f mousePosition = (sf::Vector2f)m_Window->GetMousePosition();
	sf::FloatRect bounds = shape.getGlobalBounds();

	return bounds.contains(mousePosition);
}

void ButtonComponent::SetOnClickCallback(std::function<void()> onClickCallback)
{
	this->onClickCallback = onClickCallback;
}

sf::Vector2f ButtonComponent::GetPosition() const
{
    return shape.getPosition();
}

sf::Vector2f ButtonComponent::GetSize() const
{
    return shape.getSize();
}

void ButtonComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(shape, states);
    target.draw(m_Text, states);
}
