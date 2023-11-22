#include "InsertFieldComponent.h"
#include "src/core/Managers/InputHandler.h"
#include "src/core/Managers/Resources/FontRegistry.h"

InsertFieldComponent::InsertFieldComponent(const sf::Vector2f& pos, const sf::Vector2f& size,
    const sf::Color& idleColor, const sf::Color& hoverColor,
    float outlineThickness)
    : m_Focus(false)
{
    m_Rectangle.setPosition(pos);
    m_Rectangle.setSize(size);
    m_Rectangle.setFillColor(idleColor);
    m_Rectangle.setOutlineColor(hoverColor);
    m_Rectangle.setOutlineThickness(outlineThickness);

    m_Text.SetPosition(m_Rectangle.getPosition());
}

InsertFieldComponent::~InsertFieldComponent()
{
}

void InsertFieldComponent::Update()
{
    if (IsMouseOver())
    {
        m_Rectangle.setOutlineThickness(2.0f);

        if (InputHandler::IsMouseButtonPressed(sf::Mouse::Left))
        {
            m_Focus = true;
        }
    }
    else
    {
        m_Rectangle.setOutlineThickness(0.0f);
    }

    if (m_Focus && Window::IsFocused())
    {
        if (InputHandler::IsKeyPressed(sf::Keyboard::BackSpace) && m_TextStream.str().size() > 0)
        {
            auto s = m_TextStream.str();
            s.pop_back();
            m_TextStream.str(s);
            m_Text.SetText(m_TextStream.str());
        }


        for (int key = sf::Keyboard::A; key <= sf::Keyboard::Z; ++key)
        {
            if (InputHandler::IsKeyPressed(static_cast<sf::Keyboard::Key>(key)))
            {
                char pressedChar = 'A' + (key - sf::Keyboard::A);
                m_TextStream << pressedChar;
                m_Text.SetText(m_TextStream.str());
            }
        }

        for (int key = sf::Keyboard::Num0; key <= sf::Keyboard::Num9; ++key)
        {
            if (InputHandler::IsKeyPressed(static_cast<sf::Keyboard::Key>(key)))
            {
                char pressedChar = '0' + (key - sf::Keyboard::Num0);
                m_TextStream << pressedChar;
                m_Text.SetText(m_TextStream.str());
            }
        }
        for (int key = sf::Keyboard::Numpad0; key <= sf::Keyboard::Numpad9; ++key)
        {
            if (InputHandler::IsKeyPressed(static_cast<sf::Keyboard::Key>(key)))
            {
                char pressedChar = '0' + (key - sf::Keyboard::Numpad0);
                m_TextStream << pressedChar;
                m_Text.SetText(m_TextStream.str());
            }
        }

        if (InputHandler::IsKeyPressed(sf::Keyboard::Period))
        {
            m_TextStream << '.';
            m_Text.SetText(m_TextStream.str());
        }

        if (InputHandler::IsKeyPressed(sf::Keyboard::Enter))
        {
            m_Focus = false;
        }
    }
}

void InsertFieldComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_Rectangle, states);
	target.draw(m_Text, states);
}

bool InsertFieldComponent::IsMouseOver()
{
    sf::Vector2f mousePos = (sf::Vector2f)InputHandler::GetMousePosition();
    sf::Vector2f buttonPos = m_Rectangle.getPosition();
    sf::Vector2f buttonSize = m_Rectangle.getSize();

    return mousePos.x >= buttonPos.x && mousePos.x <= buttonPos.x + buttonSize.x &&
        mousePos.y >= buttonPos.y && mousePos.y <= buttonPos.y + buttonSize.y;
}

void InsertFieldComponent::SetPosition(const sf::Vector2f& position)
{
    m_Rectangle.setPosition(position);

    float xPos = position.x + m_Rectangle.getOutlineThickness();
    float yPos = position.y + m_Rectangle.getOutlineThickness();

    m_Text.SetPosition(sf::Vector2f(xPos, yPos));
}

void InsertFieldComponent::SetFocus(bool focus)
{
    m_Focus = focus;
}
