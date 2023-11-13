#include "ClientApp.h"
#include "Window.h"

void ClientApp::Init()
{
    m_IsRunning = true;
    m_Window = new Window();
    m_Window->Create("Tic Tac Toe Online!", 800, 600);


    tempShape = new sf::RectangleShape(sf::Vector2f(100, 100));
    tempShape->setFillColor(sf::Color::Red);
    m_Window->RegisterDrawable(tempShape);
}

void ClientApp::Run()
{
    if (!m_IsRunning)
        throw std::runtime_error("ClientApp is not initialized!");

    while (m_IsRunning)
    {
        m_Window->PollEvents();
        Update();
        m_Window->Render();
        m_IsRunning = m_Window->IsOpen();
    }

    Cleanup();
}

void ClientApp::Update()
{
    tempShape->setPosition((sf::Vector2f)m_Window->GetMousePosition());
}

void ClientApp::Cleanup()
{
    for (auto& drawable : m_Window->GetDrawables())
    {
        RELEASE(drawable);
    }
    RELEASE(m_Window);
}
