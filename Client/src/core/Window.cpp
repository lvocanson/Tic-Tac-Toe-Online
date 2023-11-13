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

void Window::Render()
{
    m_Window->clear(m_ClearColor);
    for (auto drawable : m_Drawables)
    {
        m_Window->draw(*drawable);
    }
    m_Window->display();
}
