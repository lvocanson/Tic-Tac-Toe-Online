#include "ClientApp.h"
#include "Window.h"
#include "src/core/StateMachine/GameStates/EndState.h"
#include "src/core/StateMachine/GameStates/GameState.h"
#include "src/core/StateMachine/GameStates/HistoryState.h"
#include "src/core/StateMachine/GameStates/MenuState.h"
#include "src/core/StateMachine/GameStates/SelectState.h"

using namespace TicTacToe;


DWORD WINAPI PullServer(LPVOID lpParam);

void ClientApp::Init()
{
    FontRegistry::LoadFont("bold-font");

    m_IsRunning.WaitGet().operator->() = true;

    m_Window = new Window();
    m_Window->Create("Tic Tac Toe Online!", 1280, 720);
    
    m_Client = new TcpIpClient();

    m_StateMachine = new StateMachine();

    m_StateMachine->AddState("MenuState", new MenuState(m_StateMachine, m_Window));
    m_StateMachine->AddState("SelectState", new SelectState(m_StateMachine, m_Window));
    m_StateMachine->AddState("GameState", new GameState(m_StateMachine, m_Window));
    m_StateMachine->AddState("HistoryState", new HistoryState(m_StateMachine, m_Window));
    m_StateMachine->AddState("EndState", new EndState(m_StateMachine, m_Window));

    m_StateMachine->InitState("MenuState");

    m_StateMachine->Start();
}

void ClientApp::Run()
{
    if (!m_IsRunning.WaitGet().operator->())
        throw std::runtime_error("ClientApp is not initialized!");

    try
    {
        m_Client->Connect("localhost", DEFAULT_PORT);
        DebugLog("Connected to server!\n");

        m_PullServerData = (PPULLSERVERDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(PULLSERVERDATA));
        if (m_PullServerData == NULL)
        {
            ExitProcess(2);
        }

        m_PullServerData->Client = m_Client;
        m_PullServerData->StateMachine = m_StateMachine;
        m_PullServerData->IsRunning = &m_IsRunning;

        m_PullServerThread = CreateThread(
            NULL,
            0,
            PullServer,
            m_PullServerData,
            0,
            0
        );
    }
    catch (const TcpIp::TcpIpException &e)
    {
        DebugLog("Failed to connect to server: " + std::string(e.what()) + "\n");
        m_IsRunning.WaitGet().operator->() = false;
    }
    
    sf::Clock clock;
    Json j;

    while (m_IsRunning.WaitGet().operator->())
    {
        const sf::Time elapsed = clock.restart();

        m_Window->PollEvents();
        m_InputHandler.Update();

        Update(elapsed);
        m_Window->Render();
        m_IsRunning.WaitGet().operator->() = m_Window->IsOpen();
       
        if (!m_Client->IsConnected())
        {
            DebugLog("Disconnected from server!\n");
            m_IsRunning.WaitGet().operator->() = false;
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

DWORD WINAPI PullServer(LPVOID lpParam)
{
    PPULLSERVERDATA threadData = (PPULLSERVERDATA)lpParam;

    std::stringstream ss;

    while (threadData->IsRunning->WaitGet().operator->())
    {
        try
        {
            while (threadData->Client->FetchPendingData(ss))
            {
                std::string data = ss.str();
                if (!data.empty() && data[0] == '{' && data[data.size() - 1] == '}')
                {
                    Json j = Json::parse(data);
                    threadData->StateMachine->OnReceiveData(j);
                }
                else
                {
                    DebugLog("Data is not in json format!");
                }
                ss.str(std::string());
            }
        }
        catch (const TcpIp::TcpIpException& e)
        {
            DebugLog("Failed to fetch data from server: " + std::string(e.what()) + "\n");
            threadData->IsRunning->WaitGet().operator->() = false;
        }
    }
    

    return 0;
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

    CloseHandle(m_PullServerThread);
    if (m_PullServerData != NULL)
    {
        HeapFree(GetProcessHeap(), 0, m_PullServerData);
        m_PullServerData = NULL;    // Ensure address is not reused.
    }

    FontRegistry::ClearFonts();
    PlayerShapeRegistry::ClearPlayerShapes();
}