#include "TextComponent.h"
#include "ButtonComponent.h"
#include "src/core/Managers/FontRegistry.h"

TextComponent::TextComponent(const std::string& text, ButtonComponent& parentButton, const sf::Color& color, unsigned int size, TextAlignment alignment)
    : m_Alignment(alignment), m_ParentButton(parentButton)
{
    m_Text.setFont(*FontRegistry::GetFont("bold-font"));
    m_Text.setString(text);
    m_Text.setFillColor(color);
    m_Text.setCharacterSize(size);

    SetPosition(m_ParentButton.GetPosition());
}

TextComponent::TextComponent(ButtonComponent& parentButton, const sf::Color& color, unsigned int size, TextAlignment alignment)
    : m_Alignment(alignment), m_ParentButton(parentButton)
{
	m_Text.setFont(*FontRegistry::GetFont("bold-font"));
	m_Text.setFillColor(color);
	m_Text.setCharacterSize(size);

	SetPosition(m_ParentButton.GetPosition());
}

void TextComponent::SetPosition(const sf::Vector2f& position)
{
    if (m_Alignment == TextAlignment::Center)
    {
        SetPositionCentered(position);
    }
    else if (m_Alignment == TextAlignment::Right)
    {
        sf::FloatRect bounds = m_Text.getLocalBounds();
        m_Text.setPosition(position.x - bounds.width, position.y);
    }
    else
    {
        m_Text.setPosition(position);
    }
}

void TextComponent::SetPositionCentered(const sf::Vector2f& position)
{
    sf::FloatRect bounds = m_Text.getLocalBounds();
    m_Text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    m_Text.setPosition(position.x + m_ParentButton.GetSize().x / 2.0f, position.y + m_ParentButton.GetSize().y / 2.0f);
}

void TextComponent::SetTextAlignment(TextAlignment alignment)
{
    m_Alignment = alignment;
}

void TextComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_Text, states);
}
