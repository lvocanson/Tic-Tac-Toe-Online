#include "src/pch.h"
#include "ProgressBar.h"

ProgressBar::ProgressBar()
    : m_MaxValue(100)
    , m_CurrentValue(0)
{
    m_Background.setSize(sf::Vector2f(100, 20));
    m_Background.setPosition(sf::Vector2f(0, 0));
    m_Background.setFillColor(sf::Color::Black);
    m_Background.setOutlineColor(sf::Color::Color(0, 189, 156));
    m_Background.setOutlineThickness(1.0f);

    m_Foreground.setSize(m_Background.getSize());
    m_Foreground.setPosition(m_Background.getPosition());
    m_Foreground.setFillColor(sf::Color::White);

}

ProgressBar::ProgressBar(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& backgroundColor, const sf::Color& foregroundColor, const sf::Font& font, float maxValue)
    : m_MaxValue(maxValue)
    , m_CurrentValue(0)
{
    m_Background.setSize(size);
    m_Background.setPosition(position);
    m_Background.setFillColor(backgroundColor);

    m_Foreground.setSize(size);
    m_Foreground.setPosition(position);
    m_Foreground.setFillColor(foregroundColor);
}

ProgressBar::~ProgressBar()
= default;

void ProgressBar::SetSize(const sf::Vector2f& size)
{
    m_Background.setSize(size);
    m_Foreground.setSize(size);
}

void ProgressBar::SetPosition(const sf::Vector2f& position)
{
    m_Background.setPosition(position);
    m_Foreground.setPosition(position);
}

sf::Vector2f ProgressBar::GetPosition() const
{
    return m_Background.getPosition();
}

sf::Vector2f ProgressBar::GetSize() const
{
    return m_Background.getSize();
}

void ProgressBar::SetMaxValue(float maxValue)
{
    m_MaxValue = maxValue;
}

void ProgressBar::SetValue(float currentValue)
{
    if (currentValue > m_MaxValue) currentValue = m_MaxValue;
    else if (currentValue < 0) currentValue = 0;

    m_CurrentValue = currentValue;
    m_Foreground.setSize(sf::Vector2f(m_Background.getSize().x * (m_CurrentValue / m_MaxValue), m_Background.getSize().y));
}

void ProgressBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_Background, states);
    target.draw(m_Foreground, states);
}
