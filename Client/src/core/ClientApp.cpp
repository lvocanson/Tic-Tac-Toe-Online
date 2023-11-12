#include "ClientApp.h"
#include "Window.h"

void ClientApp::Init()
{
    m_IsRunning = true;
    m_Window = new Window();
    m_Window->Create("Tic Tac Toe Online!", 800, 600);
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
    // TODO: game logic
}

void ClientApp::Cleanup()
{
    RELEASE(m_Window);
}
