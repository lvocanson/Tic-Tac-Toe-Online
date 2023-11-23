#include "Window.h"


sf::RenderWindow* Window::m_Window = nullptr;
bool Window::m_IsFocused = true;

Window::Window()
    : m_ClearColor(sf::Color::Color(51, 56, 63)), m_Drawables()
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
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    
    m_Window->create(sf::VideoMode(width, height), title, sf::Style::Close, settings);

    auto image = sf::Image{};
    if (!image.loadFromFile("resources/icon.png"))
    {
        throw std::runtime_error("Failed to load icon");
    }
    m_Window->setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());

    m_Window->setVerticalSyncEnabled(true);
    m_Window->setFramerateLimit(60);
    m_Window->setMouseCursorVisible(true);
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
        case sf::Event::GainedFocus:
            m_IsFocused = true;
            break;
        case sf::Event::LostFocus:
            m_IsFocused = false;
            break;
        }
    }
}

void Window::Render()
{
    m_Window->clear(m_ClearColor);
    for (const auto drawable : m_Drawables)
    {
        m_Window->draw(*drawable);
    }
    m_Window->display();
}

void Window::ClearAllDrawables()
{
    for (auto drawable : m_Drawables)
    {
        RELEASE(drawable);
    }

    m_Drawables.clear();
}
