#include "ServerApp.h"
#include "ConsoleHelper.h"

#define ERR_CLR Color::Red // Error color
#define WRN_CLR Color::Yellow // Warning color
#define SCS_CLR Color::LightGreen // Success color
#define STS_CLR Color::LightMagenta // Status color
#define INF_CLR Color::Gray // Information color
#define DEF_CLR Color::White // Default color
#define HASH_CLR(c) HshClr(c->Address + std::to_string(c->Port)) << c->Address << ':' << c->Port
#define WEB_PFX INF_CLR << '[' << STS_CLR << "WEB" << INF_CLR << ']' << DEF_CLR << ' '

void ServerApp::Init()
{
    if (!InitGameServer())
    {
        std::cout << ERR_CLR << "Aborting app initialization." << std::endl << DEF_CLR;
        return;
    }
    else
    {
        std::cout << SCS_CLR << "Game server is listening on port " << DEFAULT_PORT << "..." << std::endl << DEF_CLR;
    }

    if (!InitWebServer())
    {
        std::cout << WEB_PFX << WRN_CLR << "Web server is unable to start, web interface will be disabled for this session." << std::endl << DEF_CLR;
    }
    else
    {
        std::cout << WEB_PFX << SCS_CLR << "Web server is listening on port " << DEFAULT_PORT + 1 << "..." << std::endl << DEF_CLR;
    }
}

void ServerApp::Run()
{
    std::cout << INF_CLR << "Press ESC to shutdown the app." << std::endl << DEF_CLR;
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
        std::cout << ERR_CLR << "The game server is unable to start: " << e.what() << std::endl << DEF_CLR;
        return false;
    }
}

void ServerApp::HandleGameServer()
{
    try
    {
        // Check for new connections / pending data / closed connections
        m_GameServer->CheckNetwork();

        // For each new connection
        ClientPtr newClient;
        while ((newClient = m_GameServer->FindNewClient()) != nullptr)
        {
            std::cout << STS_CLR << "New connection from " << HASH_CLR(newClient) << STS_CLR << " has been established." << std::endl << DEF_CLR;
            m_Lobby.AddPlayerToLobby(newClient);
        }

        // For each client with pending data
        ClientPtr sender;
        while ((sender = m_GameServer->FindClientWithPendingData()) != nullptr)
        {
            HandleRecv(sender);
        }

        // For each closed connection
        m_GameServer->CleanClosedConnections([](ClientPtr c)
            {
                std::cout << STS_CLR << "Connection from " << HASH_CLR(c) << STS_CLR << " has been closed." << std::endl << DEF_CLR;
            });
    }
    catch (const TcpIp::TcpIpException& e)
    {
        std::cout << ERR_CLR << "The game server has encountered an error: " << e.what() << std::endl << DEF_CLR;
    }
}

void ServerApp::HandleRecv(ClientPtr sender)
{
    std::string data = sender->Receive();
    std::cout << HASH_CLR(sender) << DEF_CLR << " sent " << data.size() << " bytes of data." << std::endl << DEF_CLR;
    std::cout << HASH_CLR(sender) << DEF_CLR << " sent: " << data << std::endl << DEF_CLR;
    for (ClientPtr c : m_Lobby.PlayerConnections)
    {
        if (c != sender)
        {
            c->Send(data);
        }
    }
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
        std::cout << ERR_CLR << "Game server clean up failed: " << e.what() << std::endl << DEF_CLR;
    }
}

#pragma endregion

#pragma region Web Server

bool ServerApp::InitWebServer()
{
    try
    {
        throw TcpIp::TcpIpException::Create(TcpIp::ErrorCode::WSA_StartupFailed);
        m_WebServer = new HtmlServer();
        //m_WebServer->Open(DEFAULT_PORT + 1);
        return true;
    }
    catch (const TcpIp::TcpIpException& e)
    {
        std::cout << WEB_PFX << WRN_CLR << "The web server is unable to start: " << e.what() << std::endl << DEF_CLR;
        return false;
    }
}

void ServerApp::HandleWebServer()
{
    try
    {

    }
    catch (const TcpIp::TcpIpException& e)
    {
        std::cout << WEB_PFX << ERR_CLR << "The web server has encountered an error: " << e.what() << std::endl << DEF_CLR;
    }
}

void ServerApp::HandleWebConnection()
{
    // std::cout << WEB_PFX << HASH_CLR(sender) << DEF_CLR << " has been handled." << std::endl;
}


void ServerApp::CleanUpWebServer()
{
}

#pragma endregion

