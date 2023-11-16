#pragma once

#include "src/core/Window.h"

class InputHandler
{
public:

    InputHandler();
    ~InputHandler();

    void Update();

    static bool IsKeyPressed(sf::Keyboard::Key key);
    static bool IsKeyReleased(sf::Keyboard::Key key);
    static bool IsKeyHeld(sf::Keyboard::Key key);

    /// <summary>
    /// Returns whether the given mouse button is pressed.
    /// </summary>
    static bool IsMouseButtonPressed(sf::Mouse::Button button);
    static bool IsMouseButtonReleased(sf::Mouse::Button button);
    static bool IsMouseButtonHeld(sf::Mouse::Button button);

    /// <summary>
    /// Returns the mouse position relative to the window.
    /// </summary>
    static sf::Vector2i GetMousePosition() { return sf::Mouse::getPosition(*Window::m_Window); }

private:

    static const unsigned int m_KeyCount;
    static const unsigned int m_MouseButtonCount;

    // The keyboard's state in the current- and the previous frame
    static bool m_CurrentKeyState[sf::Keyboard::KeyCount + sf::Mouse::ButtonCount];
    static bool m_PreviousKeyState[sf::Keyboard::KeyCount + sf::Mouse::ButtonCount];

};
