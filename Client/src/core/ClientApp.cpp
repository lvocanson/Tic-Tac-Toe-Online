#include "ClientApp.h"
#include "Window.h"
#include "src/core/StateMachine/AppStates/ConnectionState.h"
#include "src/core/StateMachine/AppStates/EndState.h"
#include "src/core/StateMachine/AppStates/GameState.h"
#include "src/core/StateMachine/AppStates/HistoryState.h"
#include "src/core/StateMachine/AppStates/MenuState.h"
#include "src/core/StateMachine/AppStates/LobbyState.h"
#include "src/tcp-ip/TcpIpClient.h"

using namespace TicTacToe;

void ClientApp::Init()
{
    FontRegistry::LoadFont("coolvetica.otf");

    m_IsRunning = true;

    m_TimeManager.Init();
    m_Player = new Player();

    m_Window = new Window();
    m_Window->Create("Tic Tac Toe Online!", 1280, 720);

    m_GameSettings.SetGameMode(GAMEMODE_CLASSIC);
    
    m_StateMachine = new Shared<StateMachine>();
    {
        auto lock = m_StateMachine->WaitGet();
        lock->AddState("MenuState", new MenuState(&lock.Get(), m_Window));
        lock->AddState("LobbyState", new LobbyState(&lock.Get(), m_Window));
        lock->AddState("ConnectionState", new ConnectionState(&lock.Get(), m_Window));
        lock->AddState("GameState", new GameState(&lock.Get(), m_Window));
        lock->AddState("HistoryState", new HistoryState(&lock.Get(), m_Window));
        lock->AddState("EndState", new EndState(&lock.Get(), m_Window));
        lock->InitState("MenuState");
        lock->Start();
    }

    ClientConnectionHandler::GetInstance().Init(m_StateMachine);
}

void ClientApp::Run()
{
    if (!m_IsRunning)
        throw std::runtime_error("ClientApp is not initialized!");

    while (m_IsRunning)
    {
        m_TimeManager.Tick();

        m_Window->PollEvents();
        m_InputHandler.Update();

        Update(TimeManager::GetDeltaTime());
        m_Window->Render();
        m_IsRunning &= m_Window->IsOpen();
    }

    Cleanup();
}

void ClientApp::Update(float delta)
{
    static float timeSinceLastUpdate = 0.0f;
    timeSinceLastUpdate += delta;
    auto lock = m_StateMachine->TryGet();
    if (lock.IsValid())
    {
        lock->Update(timeSinceLastUpdate);
        timeSinceLastUpdate = 0.0f;
    }
    else if (timeSinceLastUpdate > 0.3f)
    {
        auto lock2 = m_StateMachine->WaitGet();
        lock2->Update(timeSinceLastUpdate);
    }
}

void ClientApp::Cleanup()
{
    RELEASE(m_StateMachine);

    for (auto& drawable : m_Window->GetDrawables())
    {
        RELEASE(drawable);
    }

    RELEASE(m_Window);
    RELEASE(m_Player);

    ClientConnectionHandler::GetInstance().CleanUp();

        FontRegistry::ClearFonts();
    PlayerShapeRegistry::ClearPlayerShapes();
}
