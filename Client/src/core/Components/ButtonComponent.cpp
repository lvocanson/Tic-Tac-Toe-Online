#include "ButtonComponent.h"
#include "src/core/Managers/PlayerManager.h"

ButtonComponent::ButtonComponent(const sf::Vector2f pos, const sf::Vector2f size, const sf::Color& idleColor, const sf::Color& hoverColor)
    : idleColor(idleColor), hoverColor(hoverColor), onClickCallback(nullptr)
{
    shape.setPosition(pos);
    shape.setSize(size);
    shape.setFillColor(idleColor);
}

ButtonComponent::~ButtonComponent()
{
}

void ButtonComponent::Update()
{
    if (IsMouseOver())
    {
        shape.setFillColor(hoverColor);

        if (InputHandler::IsMouseButtonPressed(sf::Mouse::Left) && onClickCallback)
        {
            onClickCallback();
        }
    }
    else
    {
        shape.setFillColor(idleColor);
    }
}

void ButtonComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(shape, states);

    if (m_Text)
    {
        target.draw(*m_Text, states);
    }
}

bool ButtonComponent::IsMouseOver()
{
	sf::Vector2f mousePos = (sf::Vector2f)InputHandler::GetMousePosition();
	sf::Vector2f buttonPos = shape.getPosition();
	sf::Vector2f buttonSize = shape.getSize();

	return mousePos.x >= buttonPos.x && mousePos.x <= buttonPos.x + buttonSize.x &&
		mousePos.y >= buttonPos.y && mousePos.y <= buttonPos.y + buttonSize.y;
}

void ButtonComponent::SetPosition(const sf::Vector2f& position)
{
    shape.setPosition(position);

    if (m_Text)
    {
        m_Text->SetPosition(position);
    }
}

sf::Vector2f ButtonComponent::GetPosition() const
{
    return shape.getPosition();
}

sf::Vector2f ButtonComponent::GetSize() const
{
    return shape.getSize();
}

void ButtonComponent::SetOnClickCallback(std::function<void()> onClickCallback)
{
    this->onClickCallback = onClickCallback;
}

void ButtonComponent::SetButtonText(const std::string& text, const sf::Color& textColor, unsigned int textSize, TextAlignment textAlignment)
{
    if (!m_Text)
    {
        m_Text = std::make_unique<TextComponent>(text, *this, textColor, textSize, textAlignment);
    }
    else
    {
        m_Text->SetText(text);
        m_Text->SetColor(textColor);
        m_Text->SetCharacterSize(textSize);
        m_Text->SetTextAlignment(textAlignment);
    }
}
