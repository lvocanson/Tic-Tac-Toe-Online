#include "TextComponent.h"
#include "src/core/Managers/FontRegistry.h"

TextComponent::TextComponent() 
    : m_Alignment(Center)
{
    
    m_Text.setString("");
    m_Text.setFont(*FontRegistry::GetFont("bold-font"));
    m_Text.setFillColor(sf::Color::Black);
    m_Text.setCharacterSize(20);

    SetPosition(sf::Vector2f(0, 0));
}

TextComponent::TextComponent(const std::string& text, BaseComponent* parentComponent, const sf::Color& color, unsigned int size, TextAlignment alignment)
    : m_Alignment(alignment)
{
    m_ParentComponent = parentComponent;
    m_Text.setString(text);
    m_Text.setFont(*FontRegistry::GetFont("bold-font"));
    m_Text.setFillColor(color);
    m_Text.setCharacterSize(size);

    SetPosition(m_ParentComponent->GetPosition());
}

void TextComponent::Update()
{
}

void TextComponent::SetTextAlignment(TextAlignment alignment)
{
    m_Alignment = alignment;
}

void TextComponent::SetPosition(const sf::Vector2f& position)
{
    if (m_Alignment == TextAlignment::Center)
    {
        SetPositionCentered(position);
    }
    else if (m_Alignment == TextAlignment::Left)
    {
        m_Text.setPosition(position.x, position.y);
    }
    else
    {
        m_Text.setPosition(position);
    }
}

void TextComponent::SetText(const std::string& text)
{
    m_Text.setString(text);
}

void TextComponent::SetColor(const sf::Color& color)
{
    m_Text.setFillColor(color);
}

void TextComponent::SetCharacterSize(unsigned int size)
{
    m_Text.setCharacterSize(size);
}

void TextComponent::SetPositionCentered(const sf::Vector2f& position)
{
    sf::FloatRect bounds = m_Text.getLocalBounds();
    m_Text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    m_Text.setPosition(position.x + bounds.width / 2.0f, position.y + bounds.height / 2.0f);
}

void TextComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_Text, states);
}

sf::Vector2f TextComponent::GetPosition() const
{
    return m_Text.getPosition();
}
