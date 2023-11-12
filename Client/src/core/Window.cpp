#include "Window.h"

Window::Window()
    : m_ClearColor(sf::Color::Black), m_Drawables()
{
    m_Window = new sf::RenderWindow();
}

Window::~Window()
{
    if (m_Window->isOpen())
        m_Window->close();
    RELEASE(m_Window);
}

#pragma region Lifecycle

void Window::Create(const char* title, unsigned int width, unsigned int height)
{
    m_Window->create(sf::VideoMode(width, height), title);
    m_Window->setVerticalSyncEnabled(true);
}

void Window::PollEvents()
{
    static sf::Event event = sf::Event();
    while (m_Window->pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            m_Window->close();
            break;
        }
    }
}

bool Window::IsOpen() const
{
    return m_Window->isOpen();
}

#pragma endregion

#pragma region Rendering

void Window::Render()
{
    m_Window->clear(m_ClearColor);
    m_Window->display();
}

void Window::RegisterDrawable(sf::Drawable* drawable)
{
    m_Drawables.push_back(drawable);
}

void Window::UnregisterDrawable(sf::Drawable* drawable)
{
    m_Drawables.erase(std::remove(m_Drawables.begin(), m_Drawables.end(), drawable), m_Drawables.end());
}

#pragma endregion

#pragma region Input

bool Window::IsKeyPressed(sf::Keyboard::Key key) const
{
    return sf::Keyboard::isKeyPressed(key);
}

bool Window::IsMouseButtonPressed(sf::Mouse::Button button) const
{
    return sf::Mouse::isButtonPressed(button);
}

sf::Vector2i Window::GetMousePosition() const
{
    return sf::Mouse::getPosition(*m_Window);
}

#pragma endregion
