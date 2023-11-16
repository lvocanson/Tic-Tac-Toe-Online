#include "ServerApp.h"
#include "ConsoleHelper.h"

#define ERR_CLR Color::Red
#define WRN_CLR Color::Yellow
#define SCS_CLR Color::LightGreen
#define STS_CLR Color::LightMagenta
#define INF_CLR Color::Gray
#define DEF_CLR Color::White
#define HASH_CLR(c) HshClr(c->Address + std::to_string(c->Port)) << c->Address << ":" << c->Port

void ServerApp::Init()
{
    if (!InitGameServer())
    {
        std::cout << ERR_CLR << "Aborting app initialization." << std::endl;
        return;
    }
    else
    {
        std::cout << SCS_CLR << "Game server is listening on port " << DEFAULT_PORT << "..." << std::endl;
    }

    if (!InitWebServer())
    {
        std::cout << WRN_CLR << "Web server is unable to start, web interface will be disabled for this session." << std::endl;
    }
    else
    {
        std::cout << SCS_CLR << "Web server is listening on port " << DEFAULT_PORT + 1 << "..." << std::endl;
    }
}

void ServerApp::Run()
{
    std::cout << INF_CLR << "Press ESC to shutdown the app." << std::endl;
    while (!IsKeyPressed(27)) // 27 = ESC
    {
        HandleGameServer();
        HandleWebServer();

        // Sleep for 1ms to avoid 100% CPU usage
        Sleep(1);
    }
}

void ServerApp::CleanUp()
{
    std::cout << INF_CLR << "User requested to shutdown the app." << std::endl << DEF_CLR;
    CleanUpWebServer();
    CleanUpGameServer();
}

#pragma region Game Server

bool ServerApp::InitGameServer()
{
    try
    {
        m_GameServer = new TcpIpServer();
        m_GameServer->Open(DEFAULT_PORT);
        return true;
    }
    catch (const TcpIp::TcpIpException& e)
    {
        std::cout << INF_CLR << "The game server is unable to start: " << e.what() << std::endl;
        return false;
    }
}

void ServerApp::HandleGameServer()
{
    static std::stringstream ss;
    try
    {
        int count = m_GameServer->AcceptAllPendingConnections();
        if (count > 0)
        {
            std::cout << STS_CLR << count << " new connection" << (count > 1 ? "s" : "") << " accepted." << std::endl;
        }

        Client sender;
        while (m_GameServer->FetchPendingData(ss, sender))
        {
            HandleData(ss.str(), sender);
            ss.str(std::string()); // Clear the stringstream
        }

        count = m_GameServer->KillClosedConnections();
        if (count > 0)
        {
            std::cout << STS_CLR << count << " connection" << (count > 1 ? "s" : "") << " closed." << std::endl;
        }
    }
    catch (const TcpIp::TcpIpException& e)
    {
        std::cout << ERR_CLR << "The game server has encountered an error: " << e.what() << std::endl;
    }
}

void ServerApp::HandleData(const std::string& data, Client sender)
{
    std::cout << HASH_CLR(sender) << DEF_CLR << " sent " << data.size() << " bytes of data." << std::endl;
    m_GameServer->Send(sender, data);
}

void ServerApp::CleanUpGameServer()
{
    try
    {
        m_GameServer->Close();
        delete m_GameServer;
    }
    catch (const TcpIp::TcpIpException& e)
    {
        std::cout << ERR_CLR << "Game server clean up failed: " << e.what() << std::endl;
    }
}

#pragma endregion

#pragma region Web Server

bool ServerApp::InitWebServer()
{
    return false;
}

void ServerApp::HandleWebServer()
{
}

void ServerApp::CleanUpWebServer()
{
}

#pragma endregion

