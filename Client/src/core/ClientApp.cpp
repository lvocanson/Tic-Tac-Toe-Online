#include "ClientApp.h"
#include "Window.h"
#include "src/core/StateMachine/EndState.h"
#include "src/core/StateMachine/GameState.h"
#include "src/core/StateMachine/HistoryState.h"
#include "src/core/StateMachine/MenuState.h"
#include "src/tcp-ip/TcpIpClient.h"

using namespace TicTacToe;

void ClientApp::Init()
{
    m_IsRunning = true;
    m_Window = new Window();
    m_Window->Create("Tic Tac Toe Online!", 1280, 720);

    m_GameStateUI = new GameStateUI(m_Window);

    std::cout << "Hello World! I'm a client!\n";

    m_StateMachine = new StateMachine();

    m_StateMachine->AddState("MenuState", new MenuState(m_StateMachine, m_Window));
    m_StateMachine->AddState("GameState", new GameState(m_StateMachine, m_Window));
    m_StateMachine->AddState("HistoryState", new HistoryState(m_StateMachine, m_Window));
    m_StateMachine->AddState("EndState", new EndState(m_StateMachine, m_Window));

    m_PlayerManager.CreateNewPlayer("Player One", sf::Color(250, 92, 12));
    m_PlayerManager.CreateNewPlayer("Player Two", sf::Color(255, 194, 0));

    m_StateMachine->InitState("MenuState");
    m_StateMachine->Start();
}

void ClientApp::Run()
{
    if (!m_IsRunning)
        throw std::runtime_error("ClientApp is not initialized!");

    auto &client = TcpIpClient::GetInstance();
    try
    {
        client.Connect("localhost", DEFAULT_PORT);
        DebugLog("Connected to server!\n");
        client.Send("Hello from client!");
    }
    catch (const TcpIp::TcpIpException &e)
    {
        DebugLog("Failed to connect to server: " + std::string(e.what()) + "\n");
        m_IsRunning = false;
    }

    std::stringstream ss;
    sf::Clock clock;

    while (m_IsRunning)
    {
        const sf::Time elapsed = clock.restart();

        m_Window->PollEvents();
        Update(elapsed);
        m_Window->Render();
        m_IsRunning = m_Window->IsOpen();

        try
        {
            if (client.FetchPendingData(ss))
            {
                DebugLog("Received data from server: \n");
                DebugLog(ss.str().c_str());
                DebugLog("\n");
                ss.str(std::string());
            }
        }
        catch (const TcpIp::TcpIpException &e)
        {
            DebugLog("Failed to fetch data from server: " + std::string(e.what()) + "\n");
            m_IsRunning = false;
        }

        if (!client.IsConnected())
        {
            DebugLog("Disconnected from server!\n");
            m_IsRunning = false;
        }
    }

    client.Disconnect();
    Cleanup();
}

void ClientApp::Update(sf::Time delta)
{
    m_StateMachine->Update(delta);
}

void ClientApp::Cleanup()
{
    for (auto &drawable : m_Window->GetDrawables())
    {
        RELEASE(drawable);
    }

    RELEASE(m_Window);
    RELEASE(m_GameStateUI);

    FontRegistry::ClearFonts();
}
