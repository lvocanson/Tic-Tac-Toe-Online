#include "ClientApp.h"
#include "Window.h"
#include "src/core/StateMachine/EndState.h"
#include "src/core/StateMachine/GameState.h"
#include "src/core/StateMachine/HistoryState.h"
#include "src/core/StateMachine/MenuState.h"

using namespace TicTacToe;

void ClientApp::Init()
{
    FontRegistry::LoadFont("bold-font");

    m_IsRunning = true;
    m_Window = new Window();
    m_Window->Create("Tic Tac Toe Online!", 1280, 720);
    
    m_Client = new TcpIpClient();

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

    try
    {
        m_Client->Connect("localhost", DEFAULT_PORT);
        DebugLog("Connected to server!\n");
        
    }
    catch (const TcpIp::TcpIpException &e)
    {
        DebugLog("Failed to connect to server: " + std::string(e.what()) + "\n");
        m_IsRunning = false;
    }
    
    std::stringstream ss;
    sf::Clock clock;
    Json j;

    while (m_IsRunning)
    {
        const sf::Time elapsed = clock.restart();

        m_Window->PollEvents();
        m_InputHandler.Update();

        Update(elapsed);
        m_Window->Render();
        m_IsRunning = m_Window->IsOpen();

        try
        {
            while (m_Client->FetchPendingData(ss))
            {
                j = Json::parse(ss.str());
                ss.str(std::string());
            }
        }
        catch (const TcpIp::TcpIpException &e)
        {
            DebugLog("Failed to fetch data from server: " + std::string(e.what()) + "\n");
            m_IsRunning = false;
        }

        if (!m_Client->IsConnected())
        {
            DebugLog("Disconnected from server!\n");
            m_IsRunning = false;
        }
    }

    m_Client->Disconnect();
    Cleanup();
}

void ClientApp::Send(const std::string& data)
{
    if (!data.empty())
        m_Client->Send(data);
}

void ClientApp::Update(sf::Time delta)
{
    m_StateMachine->Update(delta.asSeconds());
}


void ClientApp::Cleanup()
{
    RELEASE(m_StateMachine);

    for (auto &drawable : m_Window->GetDrawables())
    {
        RELEASE(drawable);
    }

    RELEASE(m_Window);
    RELEASE(m_Client);

    FontRegistry::ClearFonts();
    PlayerShapeRegistry::ClearPlayerShapes();
}
