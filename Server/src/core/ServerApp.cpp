#include "ServerApp.h"
#include "ConsoleHelper.h"

#define ERR_CLR Color::Red // Error color
#define WRN_CLR Color::Yellow // Warning color
#define SCS_CLR Color::LightGreen // Success color
#define STS_CLR Color::LightMagenta // Status color
#define INF_CLR Color::Gray // Information color
#define DEF_CLR Color::White // Default color
#define HASH_CLR(c) HshClr(c->GetName()) << c->GetName()
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
    Json j;
    try
    {
        j = Json::parse(data);
    }
    catch (const std::exception& e)
    {
        std::cout << ERR_CLR << "Failed to parse JSON from " << HASH_CLR(sender) << ERR_CLR << ": " << e.what() << std::endl << DEF_CLR;
        return;
    }
    if (!j.contains("Type"))
    {
        std::cout << ERR_CLR << "Received JSON from " << HASH_CLR(sender) << ERR_CLR << " does not contain a type." << std::endl << DEF_CLR;
        return;
    }

    if (j["Type"] == "Login")
    {
        m_Players.insert(std::make_pair(sender->GetName(), j["UserName"]));
    }
    else if (j["Type"] == "LobbyList")
    {
        CreateLobbies();
        SerializeLobbiesToJson(sender);
    }
    else if (j["Type"] == "JoinLobby")
    {
        for (auto& lb : m_Lobbies)
        {
            if (j["ID"] == lb.ID)
            {
                if (!lb.IsLobbyFull())
                {
                    std::cout << INF_CLR << "Lobby is full!" << std::endl << DEF_CLR;
                    return;
                }
                lb.AddPlayerToLobby(m_Players[sender->GetName()]);
            }
        }
        SerializeLobbiesToJson(sender);
    }
    else if (j["Type"] == "LeaveLobby")
    {
        for (auto& lb : m_Lobbies)
        {
            if (j["ID"] == lb.ID)
            {
                lb.RemovePlayerFromLobby(m_Players[sender->GetName()]);
            }
        }
        SerializeLobbiesToJson(sender);
    }
    else if (j["Type"] == "Play")
    {
        // TODO: Check if player is in a lobby, check if it's their turn, check if the move is valid, send the move to the other player
    }
    else
    {
        std::cout << WRN_CLR << "Received JSON from " << HASH_CLR(sender) << WRN_CLR << " contains an unknown type." << std::endl << DEF_CLR;
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

size_t ServerApp::FindPlayer(const std::string& name)
{
    for (size_t i = 0; i < m_Lobbies.size(); ++i)
    {
        if (m_Lobbies[i].IsInLobby(name))
            return i;
    }
    return -1;
}

#pragma region Lobbying

void ServerApp::CreateLobbies()
{
    for (int i = 0; i < 3; i++)
    {
        Lobby lb;
        m_Lobbies.push_back(lb);
    }
}

void ServerApp::SerializeLobbiesToJson(ClientPtr sender)
{
    Json lobbyListJson;
    for (int i = 0; i < m_Lobbies.size(); i++)
    {
        Json lbJson;
        lbJson["ID"] = m_Lobbies[i].ID;
        lbJson["PlayerX"] = m_Lobbies[i].PlayerX.empty() ? "None" : m_Lobbies[i].PlayerX;
        lbJson["PlayerO"] = m_Lobbies[i].PlayerO.empty() ? "None" : m_Lobbies[i].PlayerO;

        lobbyListJson["Lobbies"].push_back(lbJson);
    }
    //for debug 
    std::cout << STS_CLR << lobbyListJson["Lobbies"].size() << HASH_CLR(sender) << STS_CLR << std::endl << DEF_CLR;
    sender->Send(lobbyListJson.dump());
}

#pragma endregion