#include "ServerApp.h"
#include "ConsoleHelper.h"

#define ERR_CLR Color::Red // Error color
#define WRN_CLR Color::Yellow // Warning color
#define SCS_CLR Color::LightGreen // Success color
#define STS_CLR Color::LightMagenta // Status color
#define INF_CLR Color::Gray // Information color
#define DEF_CLR Color::White // Default color
#define HASH_CLR(c) HshClr(c->GetName()) << c->GetName()
#define WEB_PFX INF_CLR << '[' << STS_CLR << "WEB" << INF_CLR << "] " // Web server prefix

void ServerApp::Init()
{
    if (!InitGameServer())
    {
        std::cout << ERR_CLR << "Aborting app initialization." << std::endl;
        return;
    }

    if (!InitWebServer())
    {
        std::cout << WEB_PFX << WRN_CLR << "Web server is unable to start, web interface will be disabled for this session." << std::endl;
    }

    auto addr = TcpIp::IpAddress::GetLocalAddress();
    std::cout << INF_CLR << "==> Servers Local Address: " << SCS_CLR << addr.ToString() << std::endl << DEF_CLR;
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
    std::cout << Color::Cyan << "=========== Starting Game Server Initialization ===========" << std::endl << INF_CLR;
    try
    {
        m_GameServer = new TcpIpServer();
        m_GameServer->Open(DEFAULT_PORT);
        std::cout << "Game server is listening on port " << DEFAULT_PORT << "..." << std::endl;
    }
    catch (const TcpIp::TcpIpException& e)
    {
        std::cout << ERR_CLR << "The game server is unable to start: " << e.what() << std::endl << DEF_CLR;
        return false;
    }

    std::cout << "Creating lobbies..." << std::endl;
    CreateLobbies();
    std::cout << "Lobbies created." << std::endl;

    std::cout << Color::Cyan << "=========== Game Server Initialization Complete ===========" << std::endl << DEF_CLR;
    return true;
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
    Json receivedData;
    try
    {
        receivedData = Json::parse(data);
    }
    catch (const std::exception& e)
    {
        std::cout << ERR_CLR << "Failed to parse JSON from " << HASH_CLR(sender) << ERR_CLR << ": " << e.what() << std::endl << DEF_CLR;
        return;
    }
    if (!receivedData.contains("Type"))
    {
        std::cout << ERR_CLR << "Received JSON from " << HASH_CLR(sender) << ERR_CLR << " does not contain a type." << std::endl << DEF_CLR;
        return;
    }

    if (receivedData["Type"] == "Login")
    {
        m_Players.insert(std::make_pair(sender->GetName(), receivedData["UserName"]));
    }////////// Lobby State //////////
    else if (receivedData["Type"] == "GetLobbyList")
    {
        SerializeLobbiesToJson(sender);
    }
    else if (receivedData["Type"] == "JoinLobby")
    {
        for (auto& lb : m_Lobbies)
        {
            if (receivedData["ID"] == lb.ID)
            {
                if (lb.IsInLobby(m_Players[sender->GetName()]))
                {
                    return;
                }
                if (lb.IsLobbyFull())
                {
                    return;
                }

                lb.AddPlayerToLobby(m_Players[sender->GetName()]);
                Json j;
                j["Type"] = "IsInLobby";
                j["CurrentLobbyID"] = lb.ID;
                sender->Send(j.dump());

                if (lb.IsLobbyFull())
                {
                    Json j;
                    j["Type"] = "OnUpdateLobby";
                    sender->Send(j.dump());
                    return;
                }
            }
        }
        SerializeLobbiesToJson(sender);
    }
    else if (receivedData["Type"] == "LeaveLobby")
    {
        for (auto& lb : m_Lobbies)
        {
            if (receivedData["ID"] == lb.ID)
            {
                lb.RemovePlayerFromLobby(m_Players[sender->GetName()]);
            }
        }
        SerializeLobbiesToJson(sender);
    }////////// Game State //////////
    else if (receivedData["Type"] == "StartGame")
    {
        if (receivedData.contains("StartedLobbyID"))
        {
            for (auto& lb : m_Lobbies)
            {
                if (receivedData["StartedLobbyID"] == lb.ID)
                {
                    m_StartedGames.insert({lb.ID, &lb});
                }
            }
        }
    }
    else if (receivedData["Type"] == "GetPlayerInfo")
    {
        Json j;
        j["Type"] = "SetPlayerShape";
        j["PlayerX"] = m_StartedGames[receivedData["LobbyID"]]->PlayerX;
        j["PlayerO"] = m_StartedGames[receivedData["LobbyID"]]->PlayerO;
        sender->Send(j.dump());
    }
    else if (receivedData["Type"] == "OpponentMove")
    {
        Json j;
        j["Type"] = "OpponentMove";
        j["PlayerMove"] = receivedData["PlayerMove"];

        sender->Send(j.dump());
    }
    else
    {
        std::cout << WRN_CLR << "Received JSON from " << HASH_CLR(sender) << WRN_CLR << " contains an unknown type." << std::endl << DEF_CLR;
    }
}

void ServerApp::CleanUpGameServer()
{
    std::cout << Color::Cyan << "============== Starting Game Server Clean Up ==============" << std::endl;
    try
    {
        m_GameServer->Close();
        delete m_GameServer;
    }
    catch (const TcpIp::TcpIpException& e)
    {
        std::cout << ERR_CLR << "Game server clean up failed: " << e.what() << std::endl << DEF_CLR;
    }
    std::cout << Color::Cyan << "============== Game Server Clean Up Complete ==============" << std::endl << DEF_CLR;
}

#pragma endregion

#pragma region Web Server

bool ServerApp::InitWebServer()
{
    std::cout << WEB_PFX << Color::Cyan << "===== Starting Web Server Initialization  ===========" << std::endl << DEF_CLR;
    try
    {
        m_WebServer = new HtmlServer();
        m_WebServer->Open(DEFAULT_PORT + 1);
        std::cout << WEB_PFX << "Web server is listening on port " << DEFAULT_PORT + 1 << "..." << std::endl;
    }
    catch (const TcpIp::TcpIpException& e)
    {
        std::cout << WEB_PFX << WRN_CLR << "Web server is unable to start: " << e.what() << std::endl << DEF_CLR;
        RELEASE(m_WebServer);
        return false;
    }
    std::cout << WEB_PFX << Color::Cyan << "===== Web Server Initialization Complete  ===========" << std::endl << DEF_CLR;
    return true;
}

void ServerApp::HandleWebServer()
{
    try
    {
        m_WebServer->CheckNetwork();

        WebClientPtr newClient;
        while ((newClient = m_WebServer->FindNewClient()) != nullptr)
        {
            std::cout << WEB_PFX << STS_CLR << "New connection from " << HASH_CLR(newClient) << STS_CLR << " has been established." << std::endl << DEF_CLR;
        }

        WebClientPtr sender;
        while ((sender = m_WebServer->FindClientWithPendingData()) != nullptr)
        {
            HandleWebConnection(sender);
        }

        m_WebServer->CleanClosedHtmlConns([](WebClientPtr c)
            {
                std::cout << WEB_PFX << STS_CLR << "Connection from " << HASH_CLR(c) << STS_CLR << " has been closed." << std::endl << DEF_CLR;
            });
    }
    catch (const TcpIp::TcpIpException& e)
    {
        std::cout << WEB_PFX << ERR_CLR << "The web server has encountered an error: " << e.what() << std::endl << DEF_CLR;
    }
}

void ServerApp::HandleWebConnection(WebClientPtr sender)
{
    constexpr int maxWouldBlockErrors = 10;
    int wbe = 0;
    std::string data;
    while (true)
    {
        try
        {
            data = sender->Receive();
            break;
        }
        catch (const TcpIp::TcpIpException& e)
        {
            if (e.Context == WSAEWOULDBLOCK && wbe < maxWouldBlockErrors)
            {
                ++wbe; // "Try again" error, ignore
                continue;
            }

            std::cout << WEB_PFX << ERR_CLR << "Error while receiving data from " << HASH_CLR(sender) << DEF_CLR << ": " << e.what() << std::endl;
            return;
        }
    }

    if (data.starts_with("GET "))
    {
        std::string page = data.substr(4, data.find(' ', 4) - 4);
        std::cout << WEB_PFX << HASH_CLR(sender) << DEF_CLR << " sent a GET request for " << page << ". ";

        if (page == "/")
        {
            std::cout << "Sending root page." << std::endl;
            sender->Send("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"

                // TODO: Send the actual page, lobbies with href to /watch/lobbyid

                "<html><body><h1>Hello " + sender->Address + ":" + std::to_string(sender->Port) + "</h1></body></html>");
        }
        else if (page == "/favicon.ico")
        {
            // We don't have a favicon, so just send a 404
            std::cout << "Sending 404." << std::endl;
            sender->Send("HTTP/1.1 404 Not Found\r\n\r\n");
        }

        // TODO: page.starts_with("/watch/") -> send the watch page for the lobby

        /*
            The client should get something like this: (maybe with a link to the root page)

            Lobby ID: 123456
            Player X: "Player 1"
            Player O: "Player 2"
            Turn: X


             X | O | X
            -----------
             X | X | O
            -----------
             O | X | O

        */

        else
        {
            std::cout << "Redirecting to root page." << std::endl;
            sender->Send("HTTP/1.1 301 Moved Permanently\r\nLocation: /\r\n\r\n");
        }
    }
    else
    {
        std::cout << WEB_PFX << HASH_CLR(sender) << DEF_CLR << " sent an unknown request." << std::endl;
    }

    // Close the connection
    sender->Kick();
}

void ServerApp::CleanUpWebServer()
{
    if (!m_WebServer) return;

    std::cout << WEB_PFX << Color::Cyan << "======== Starting Web Server Clean Up  ==============" << std::endl << DEF_CLR;
    try
    {
        m_WebServer->Close();
        delete m_WebServer;
    }
    catch (const TcpIp::TcpIpException& e)
    {
        std::cout << WEB_PFX << ERR_CLR << "Web server clean up failed: " << e.what() << std::endl << DEF_CLR;
    }
    std::cout << WEB_PFX << Color::Cyan << "======== Web Server Clean Up Complete  ==============" << std::endl << DEF_CLR;
}

#pragma endregion

#pragma region Lobbying

size_t ServerApp::FindPlayer(const std::string& name)
{
    for (size_t i = 0; i < m_Lobbies.size(); ++i)
    {
        if (m_Lobbies[i].IsInLobby(name))
            return i;
    }
    return -1;
}

void ServerApp::CreateLobbies()
{
    for (int i = 0; i < 3; i++)
    {
        m_Lobbies.emplace_back(Lobby());
    }
}

void ServerApp::SerializeLobbiesToJson(ClientPtr sender)
{
    Json lobbyListJson;
    lobbyListJson["Type"] = "Lobby";
    for (int i = 0; i < m_Lobbies.size(); i++)
    {
        Json lbJson;
        lbJson["ID"] = m_Lobbies[i].ID;
        lbJson["PlayerX"] = m_Lobbies[i].PlayerX.empty() ? "" : m_Lobbies[i].PlayerX;
        lbJson["PlayerO"] = m_Lobbies[i].PlayerO.empty() ? "" : m_Lobbies[i].PlayerO;

        lobbyListJson["Lobbies"].push_back(lbJson);
    }
    sender->Send(lobbyListJson.dump());
}

#pragma endregion

void ServerApp::SerializeGameDataToJson(ClientPtr sender)
{
    Json gameData;


    sender->Send(gameData.dump());
}