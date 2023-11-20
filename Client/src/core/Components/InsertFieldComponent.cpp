#include "InsertFieldComponent.h"
#include "src/core/Managers/InputHandler.h"
#include "src/core/Managers/Resources/FontRegistry.h"

InsertFieldComponent::InsertFieldComponent()
    : m_CharacterLimit(15)
    , m_Focus(false)
    , m_CursorTimer(0.0f)
{
    SetPosition(sf::Vector2f(0.0f, 0.0f));

    m_Rectangle.setSize(sf::Vector2f(300, 50));
    m_Rectangle.setFillColor(sf::Color(171, 171, 171));
    m_Rectangle.setOutlineColor(sf::Color::White);
    m_Rectangle.setOutlineThickness(1.0f);

    m_Label.SetText("InsertField");
    m_Label.SetColor(sf::Color(171, 171, 171));
    m_Label.SetCharacterSize(16);

    m_Cursor.SetText("|");
    m_Cursor.SetColor(sf::Color::Black);
}

InsertFieldComponent::InsertFieldComponent(const sf::Vector2f& pos, const sf::Vector2f& size,
    const sf::Color& idleColor, const sf::Color& hoverColor,
    float outlineThickness, unsigned int characterLimit)
    : m_CharacterLimit(characterLimit)
    , m_Focus(false)
    , m_CursorTimer(0.0f)
{
    SetPosition(sf::Vector2f(0.0f, 0.0f));

    m_Rectangle.setSize(size);
    m_Rectangle.setFillColor(idleColor);
    m_Rectangle.setOutlineColor(hoverColor);
    m_Rectangle.setOutlineThickness(outlineThickness);
}

InsertFieldComponent::~InsertFieldComponent()
= default;

void InsertFieldComponent::Update(float dt)
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
        m_Focus = false;
    }

    if (m_CursorTimer > m_BlinkTime)
    {
        m_CursorTimer = 0.0f;
        m_Cursor.SetVisible(!m_Cursor.IsVisible());
    }
    else
    {
        m_CursorTimer += dt;
    }

    if (m_Focus && Window::IsFocused())
    {
        if (InputHandler::IsKeyPressed(sf::Keyboard::BackSpace) && GetTextSize() > 0)
        {
            auto s = m_TextStream.str();
            s.pop_back();
            m_TextStream.str(s);
            m_Text.SetText(m_TextStream.str());
        }


        if (GetTextSize() < m_CharacterLimit)
        {
            for (int key = sf::Keyboard::A; key <= sf::Keyboard::Z; ++key)
            {
                if (InputHandler::IsKeyPressed(static_cast<sf::Keyboard::Key>(key)))
                {
                    char pressedChar = 'A' + (key - sf::Keyboard::A);
                    AppendCharacter(pressedChar);
                }
            }

            for (int key = sf::Keyboard::Num0; key <= sf::Keyboard::Num9; ++key)
            {
                if (InputHandler::IsKeyPressed(static_cast<sf::Keyboard::Key>(key)))
                {
                    char pressedChar = '0' + (key - sf::Keyboard::Num0);
                    AppendCharacter(pressedChar);
                }
            }
            for (int key = sf::Keyboard::Numpad0; key <= sf::Keyboard::Numpad9; ++key)
            {
                if (InputHandler::IsKeyPressed(static_cast<sf::Keyboard::Key>(key)))
                {
                    char pressedChar = '0' + (key - sf::Keyboard::Numpad0);
                    AppendCharacter(pressedChar);
                }
            }

            if (InputHandler::IsKeyPressed(sf::Keyboard::Period))
            {
                AppendCharacter('.');
            }
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
    target.draw(m_Label, states);

    if (m_Cursor.IsVisible())
        target.draw(m_Cursor, states);
}

bool InsertFieldComponent::IsMouseOver()
{
    const sf::Vector2f mousePos = (sf::Vector2f)InputHandler::GetMousePosition();
    const sf::Vector2f buttonPos = m_Rectangle.getPosition();
    const sf::Vector2f buttonSize = m_Rectangle.getSize();

    return mousePos.x >= buttonPos.x && mousePos.x <= buttonPos.x + buttonSize.x &&
        mousePos.y >= buttonPos.y && mousePos.y <= buttonPos.y + buttonSize.y;
}

void InsertFieldComponent::AppendCharacter(const char& c)
{
    m_TextStream << c;
    m_Text.SetText(m_TextStream.str());
    m_Cursor.SetPosition(m_Text.GetPosition() + sf::Vector2f(GetTextSize() * m_Text.GetCharacterSize(), 0));
}

void InsertFieldComponent::SetPosition(const sf::Vector2f& position)
{
    m_Rectangle.setPosition(position);

    m_Label.SetPosition(m_Rectangle.getPosition() - sf::Vector2f(0, m_Label.GetSize().y + m_Rectangle.getOutlineThickness() + 5));

    const float xPos = position.x + m_Rectangle.getOutlineThickness();
    const float yPos = position.y + m_Rectangle.getOutlineThickness();

    m_Text.SetPosition(sf::Vector2f(xPos, yPos));
    m_Cursor.SetPosition(m_Text.GetPosition() + sf::Vector2f( GetTextSize() * m_Text.GetCharacterSize(), 0));
}

