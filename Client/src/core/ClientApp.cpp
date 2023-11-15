#include "ClientApp.h"
#include "Window.h"
#include "src/core/StateMachine/EndState.h"
#include "src/core/StateMachine/GameState.h"
#include "src/core/StateMachine/HistoryState.h"
#include "src/core/StateMachine/MenuState.h"

void ClientApp::Init()
{
    m_IsRunning = true;
    m_Window = new Window();
    m_Window->Create("Tic Tac Toe Online!", 1280, 720);
    
    std::cout << "Hello World! I'm a client!\n";
    
    m_StateMachine = new StateMachine();

    m_StateMachine->AddState("MenuState", new MenuState(m_StateMachine, m_Window));
    m_StateMachine->AddState("GameState", new GameState(m_StateMachine, m_Window));
    m_StateMachine->AddState("HistoryState", new HistoryState(m_StateMachine, m_Window));
    m_StateMachine->AddState("EndState", new EndState(m_StateMachine, m_Window));

    m_StateMachine->InitState("MenuState");
    m_StateMachine->Start();
}



void ClientApp::Run()
{
    if (!m_IsRunning)
        throw std::runtime_error("ClientApp is not initialized!");

    sf::Clock clock;

    while (m_IsRunning)
    {
        const sf::Time elapsed = clock.restart();

        m_Window->PollEvents();
        Update(elapsed);
        m_Window->Render();
        m_IsRunning = m_Window->IsOpen();
    }

    Cleanup();
}

void ClientApp::Update(sf::Time delta)
{
    m_StateMachine->Update(delta.asSeconds());
}


void ClientApp::Cleanup()
{
    for (auto& drawable : m_Window->GetDrawables())
    {
        RELEASE(drawable);
    }

    RELEASE(m_Window);
}
