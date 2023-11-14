#include "ClientApp.h"
#include "Window.h"
#include "src/tcp-ip/TcpIpClient.h"

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

    auto& client = TcpIpClient::GetInstance();
    try
    {
        client.Connect("localhost", DEFAULT_PORT);
        DebugLog("Connected to server!\n");
        client.Send("Hello from client!");
    }
    catch (const TcpIp::TcpIpException& e)
    {
        DebugLog("Failed to connect to server: " + std::string(e.what()) + "\n");
        m_IsRunning = false;
    }

    std::stringstream ss;
    while (m_IsRunning)
    {
        m_Window->PollEvents();
        Update();
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
        catch (const TcpIp::TcpIpException& e)
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
