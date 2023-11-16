#include "InputHandler.h"

bool InputHandler::m_CurrentKeyState[sf::Keyboard::KeyCount + sf::Mouse::ButtonCount];
bool InputHandler::m_PreviousKeyState[sf::Keyboard::KeyCount + sf::Mouse::ButtonCount];

const unsigned int InputHandler::m_MouseButtonCount = sf::Mouse::ButtonCount;
const unsigned int InputHandler::m_KeyCount = sf::Keyboard::KeyCount;


InputHandler::InputHandler()
{

}

void InputHandler::Update()
{
    for (unsigned int i = 0; i < m_KeyCount + m_MouseButtonCount; i++)
    {
        if (i > m_KeyCount)
        {
            // Save the mouse's state from the previous frame
            m_PreviousKeyState[i] = m_CurrentKeyState[i];

            // And save the mouse's state in the current frame
            m_CurrentKeyState[i] = sf::Mouse::isButtonPressed((sf::Mouse::Button)(i - m_KeyCount - 1));
        }
        else 
        {
            // Save the keyboard's state from the previous frame
            m_PreviousKeyState[i] = m_CurrentKeyState[i];

            // And save the keyboard's state in the current frame
            m_CurrentKeyState[i] = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)i);
        }
    }
}

InputHandler::~InputHandler()
{
}

bool InputHandler::IsKeyPressed(sf::Keyboard::Key key)
{
    return (m_CurrentKeyState[key] && !m_PreviousKeyState[key]);
}

bool InputHandler::IsKeyReleased(sf::Keyboard::Key key)
{
    return (!m_CurrentKeyState[key] && m_PreviousKeyState[key]);
}

bool InputHandler::IsKeyHeld(sf::Keyboard::Key key)
{
    return m_CurrentKeyState[key];
}

bool InputHandler::IsMouseButtonPressed(sf::Mouse::Button button)
{
    return (m_CurrentKeyState[m_KeyCount + button + 1] && !m_PreviousKeyState[m_KeyCount + button + 1]);
}

bool InputHandler::IsMouseButtonReleased(sf::Mouse::Button button)
{
    return (!m_CurrentKeyState[m_KeyCount + button + 1] && m_PreviousKeyState[m_KeyCount + button + 1]);
}

bool InputHandler::IsMouseButtonHeld(sf::Mouse::Button button)
{
    return (m_CurrentKeyState[m_KeyCount + button + 1]);
}

