#include "ServerApp.h"
#include "ConsoleHelper.h"
#include "tcp-ip/ClientMessages.h"
#include "tcp-ip/ServerMessages.h"

#define ERR_CLR Color::Red // Error color
#define WRN_CLR Color::Yellow // Warning color
#define SCS_CLR Color::LightGreen // Success color
#define STS_CLR Color::LightMagenta // Status color
#define INF_CLR Color::Gray // Information color
#define DEF_CLR Color::White // Default color
#define HASH_CLR(c) HshClr(c->GetName()) << c->GetName()
#define HASH_STRING_CLR(c) HshClr(c) << c
#define WEB_PFX INF_CLR << '[' << STS_CLR << "WEB" << INF_CLR << "] " // Web server prefix


constexpr int MAXIMUM_LOBBIES = 6;

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
    std::cout << INF_CLR << "=> Game Server Phrase: " << SCS_CLR << addr.ToPhrase() << std::endl;
    std::cout << INF_CLR << "=> Web Server Address: " << SCS_CLR << "http://" << addr.ToString() << ':' << DEFAULT_PORT + 1 << "/" << DEF_CLR << std::endl;
}

void ServerApp::Run()
{
    std::cout << INF_CLR << "Press ESC to shutdown the app." << std::endl << DEF_CLR << std::endl;
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
    std::cout << std::endl << INF_CLR << "User requested to shutdown the app." << std::endl << DEF_CLR;

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
        m_GameServer->CleanClosedConnections([this](ClientPtr c)
        {
            const auto& player = m_Players[c->GetName()];
            if (!player.empty())
            {
                bool wasInLobby = false;
                for (const auto lb : m_Lobbies)
                {
                    if (lb->IsInLobby(player))
                    {
                        lb->RemovePlayerFromLobby(player);
                        wasInLobby = true;
                    }
                }

                UnregisterPlayerFromServer(player);

                if (wasInLobby)
                    RefreshLobbyListToPlayers();
            }

            std::cout << STS_CLR << "Connection from " << HASH_CLR(c) << STS_CLR << " has been closed." << std::endl << DEF_CLR;
        });
    }
    catch (const TcpIp::TcpIpException& e)
    {
        std::cout << ERR_CLR << "The game server has encountered an error: " << e.what() << std::endl << DEF_CLR;
    }
}

void ServerApp::UnregisterPlayerFromServer(const std::string& player)
{
    m_Players.erase(player);
    std::cout << STS_CLR << "Unregistered player: " << HASH_STRING_CLR(player) << STS_CLR << " from server." << std::endl << DEF_CLR;
}

void ServerApp::RefreshLobbyListToPlayers()
{
    const size_t playerCount = m_Players.size();
    unsigned int playerInLobbyCount = 0;

    for (const auto& lobby : m_Lobbies)
    {
        playerInLobbyCount += lobby->PlayerCount;
    }

    // Send the lobby list to all players that are not in a lobby
    if (playerCount > playerInLobbyCount)
    {
        Message<MsgType::LobbyList> toSend;

        toSend.LobbiesData.reserve(m_Lobbies.size());
        for (const auto& lb : m_Lobbies)
        {
            toSend.LobbiesData.emplace_back(lb->Data);
        }

        const std::string message = toSend.Serialize().dump();

        for (auto& [adressIP, player] : m_Players)
        {
            if (IsPlayerInLobby(player)) continue;

            if (const ClientPtr client = m_GameServer->GetClientByName(adressIP))
            {
                client->Send(message);
            }
        }

        std::cout << INF_CLR << "Refresh lobby sent to players " << std::endl << DEF_CLR;
    }
}

void ServerApp::HandleRecv(ClientPtr sender)
{
    std::string data = sender->Receive();
    Json parsedData;
    try
    {
        parsedData = Json::parse(data);
    }
    catch (const std::exception& e)
    {
        std::cout << ERR_CLR << "Failed to parse JSON from " << HASH_CLR(sender) << ERR_CLR << ": " << e.what() << std::endl << DEF_CLR;
        return;
    }
    MsgType type = Message<>::GetType(parsedData);

    // TODO: Add safeguards before accessing members of this class. (e.g. check if the player is in a lobby before accessing m_StartedGames)
    using enum MsgType;
    switch (type)
    {
    case Login:
    {
        Message<Login> msg(parsedData);
        if (!m_Players.contains(sender->GetName()))
        {
            m_Players.insert({ sender->GetName(), msg.Username });
            std::cout << INF_CLR << "Registered player: " << HASH_STRING_CLR(msg.Username) << INF_CLR << " into server." << std::endl << DEF_CLR;
        }
        else
        {
            std::cout << WRN_CLR << "Player " << HASH_STRING_CLR(m_Players[sender->GetName()]) << WRN_CLR << " tried to login again." << std::endl << DEF_CLR;
            return;
        }
        break;
    }
    case Disconnect:
    {
        std::string& username = m_Players.at(sender->GetName());
        for (const auto& lb : m_Lobbies)
        {
            // If the player is in a lobby, remove him from it
            if (lb->IsInLobby(username))
            {
                lb->RemovePlayerFromLobby(username);
                std::cout << INF_CLR << "Player " << HASH_STRING_CLR(username) << INF_CLR << " has left lobby: " << lb->Data.ID << std::endl << DEF_CLR;

                RefreshLobbyListToPlayers();

                if (m_StartedGames.contains(lb->Data.ID) && lb->IsLobbyEmpty())
                {
                    m_StartedGames[lb->Data.ID] = nullptr;
                    m_StartedGames.erase(lb->Data.ID);
                    std::cout << INF_CLR << "Closing game " << lb->Data.ID << "..." << std::endl << DEF_CLR;
                }
            }
        }
        break;
    }
    case FetchLobbyList:
    {
        Message<LobbyList> toSend;
        toSend.LobbiesData.reserve(m_Lobbies.size());
        for (auto& lb : m_Lobbies)
        {
            toSend.LobbiesData.emplace_back(lb->Data);
        }
        sender->Send(toSend.Serialize().dump());
        std::cout << INF_CLR << "Lobby list sent to " << HASH_CLR(sender) << std::endl << DEF_CLR;
        break;
    }
    case FetchGameHistoryList:
    {
        if (m_SavedGames.empty())
        {
            break;
        }

        Message<GameHistoryList> toSend;
        toSend.GameHistory.reserve(m_SavedGames.size());
        for (const auto game : m_SavedGames)
        {
            toSend.GameHistory.push_back(game);
        }

        sender->Send(toSend.Serialize().dump());
        std::cout << INF_CLR << "Game History list sent to " << HASH_CLR(sender) << std::endl << DEF_CLR;

        break;
    }
    case TryToJoinLobby:
    {
        Message<TryToJoinLobby> msg(parsedData);
        bool joined = false;

        // Find the lobby with the given ID
        for (auto& lb : m_Lobbies)
        {
            if (msg.LobbyId != lb->Data.ID) continue;
            const std::string& playerName = m_Players[sender->GetName()];

            // Check if the player can join it
            if (lb->IsInLobby(playerName))
            {
                std::cout << WRN_CLR << "Player " << HASH_STRING_CLR(playerName) << WRN_CLR << " tried to join lobby: " << INF_CLR << msg.LobbyId << WRN_CLR << " but he's already in." << std::endl << DEF_CLR;
                joined = false;
                break;
            }
            else if (lb->IsLobbyFull())
            {
                std::cout << WRN_CLR << "Player " << HASH_STRING_CLR(playerName) << WRN_CLR << " tried to join lobby: " << INF_CLR << msg.LobbyId << WRN_CLR << " but it's full." << std::endl << DEF_CLR;
                joined = false;
                break;
            }

            lb->AddPlayerToLobby(playerName);
            joined = true;
            std::cout << INF_CLR << "[Lobby " << msg.LobbyId << " ] Player " << HASH_STRING_CLR(playerName) << INF_CLR << " has joined." << std::endl << DEF_CLR;

            sender->Send(Message<AcceptJoinLobby>().Serialize().dump());
            std::cout << INF_CLR << "Lobby confirmation sent to " << HASH_CLR(sender) << std::endl << DEF_CLR;

            // Create the lobby game if it doesn't exist
            if (!m_StartedGames.contains(lb->Data.ID))
                m_StartedGames.insert({lb->Data.ID, lb});

            RefreshLobbyListToPlayers();
            break;
        }

        // Send rejection message
        if (!joined)
        {
            sender->Send(Message<RejectJoinLobby>().Serialize().dump());
            std::cout << INF_CLR << "[Lobby " << msg.LobbyId << " ] Rejected " << HASH_CLR(sender) << std::endl << DEF_CLR;
        }
        break;
    }
    case OnEnterLobby:
    {
        Message<OnEnterLobby> msg(parsedData);

        for (auto& lb : m_Lobbies)
        {
            if (lb->Data.ID != msg.LobbyId) continue;

            if (lb->IsLobbyFull())
            {
                std::string startingPlayer = rand() % 100 <= 50 ? lb->Data.PlayerX : lb->Data.PlayerO;

                Message<GameStarted> toSend;
                toSend.GameMode = lb->Data.GameMode;
                toSend.PlayerO = lb->Data.PlayerO;
                toSend.PlayerX = lb->Data.PlayerX;
                toSend.StartPlayer = startingPlayer;

                std::string opponentName = lb->GetOpponentName(m_Players[sender->GetName()]);

                int i = 0;
                for (auto& [adressIP, player] : m_Players)
                {
                    if (lb->IsInLobby(player))
                    {
                        m_GameServer->GetClientByName(adressIP)->Send(toSend.Serialize().dump());
                        i++;
                    }

                    if (i == 2) break;
                }

                std::cout << STS_CLR << "Started game in lobby  " << INF_CLR << lb->Data.ID << std::endl << DEF_CLR;
            }

            break;
        }
        break;
    }
    case MakeMove:
    {
        Message<MakeMove> msg(parsedData);
        Lobby* lb = m_StartedGames[msg.LobbyId];
        std::string& playerName = m_Players[sender->GetName()];

        // Check if move is valid
        if (!lb->Board.IsCellEmpty(msg.Cell))
        {
            std::cout << WRN_CLR << "[Lobby " << msg.LobbyId << " ] Player " << HASH_STRING_CLR(playerName) << WRN_CLR << " tried to make an invalid move." << std::endl << DEF_CLR;
            sender->Send(Message<DeclineMakeMove>().Serialize().dump());
            break;
        }

        lb->Board[msg.Cell] = msg.Piece;
        lb->AddPlayerMove(playerName, msg.Piece, msg.Cell);

        // Send response message to both players
        Message<AcceptMakeMove> acceptMsg;
        acceptMsg.Cell = msg.Cell;
        acceptMsg.Piece = msg.Piece;

        int i = 0;
        for (auto& [adressIP, player] : m_Players)
        {
            if (lb->IsInLobby(player))
            {
                m_GameServer->GetClientByName(adressIP)->Send(acceptMsg.Serialize().dump());
                i++;
            }

            if (i == 2) break;
        }
        std::cout << INF_CLR << "[Lobby " << msg.LobbyId << " ] Player " << HASH_STRING_CLR(playerName) << INF_CLR << " made a move." << std::endl << DEF_CLR;

        // Check if the game is over
        TicTacToe::Piece winner = lb->Board.IsThereAWinner();
        if (winner != TicTacToe::Piece::Empty)
        {
            Message<GameOver> overMsg;
            overMsg.Winner = winner == TicTacToe::Piece::X ? lb->Data.PlayerX : lb->Data.PlayerO;
            overMsg.Piece = winner;

            int i = 0;
            for (auto& [adressIP, player] : m_Players)
            {
                if (lb->IsInLobby(player))
                {
                    m_GameServer->GetClientByName(adressIP)->Send(overMsg.Serialize().dump());
                    i++;
                }

                if (i == 2) break;
            }

            m_SavedGames.emplace_back(GameData(lb->CurrentGame, lb->Data.PlayerX, lb->Data.PlayerO));
            lb->ResetGame();

            std::cout << INF_CLR << "[Lobby " << msg.LobbyId << " ] Player " << HASH_STRING_CLR(playerName) << INF_CLR << " won the game." << std::endl << DEF_CLR;
        }
        else if (lb->Board.IsFull())
        {
            Message<GameOver> overMsg;
            overMsg.Winner = "Nobody";
            overMsg.IsDraw = true;

            int i = 0;
            for (auto& [adressIP, player] : m_Players)
            {
                if (lb->IsInLobby(player))
                {
                    m_GameServer->GetClientByName(adressIP)->Send(overMsg.Serialize().dump());
                    i++;
                }

                if (i == 2) break;
            }

            lb->ResetGame();

            std::cout << INF_CLR << "[Lobby " << msg.LobbyId << " ] The game ended in a draw." << std::endl << DEF_CLR;
        }

        break;
    }
    case LeaveLobby:
    {
        Message<LeaveLobby> msg(parsedData);
        Lobby* lb = m_StartedGames[msg.LobbyId];
        std::string opponentName = lb->GetOpponentName(msg.PlayerName);

        lb->RemovePlayerFromLobby(msg.PlayerName);
        std::cout << INF_CLR << "[Lobby " << msg.LobbyId << " ] Player " << HASH_STRING_CLR(msg.PlayerName) << INF_CLR << " has left." << std::endl << DEF_CLR;

        RefreshLobbyListToPlayers();

        if (m_StartedGames.contains(lb->Data.ID) && lb->IsLobbyEmpty())
        {
            m_StartedGames[lb->Data.ID] = nullptr;
            m_StartedGames.erase(lb->Data.ID);
            std::cout << INF_CLR << "Closing game " << lb->Data.ID << "..." << std::endl << DEF_CLR;
        }

        if (!lb->IsLobbyEmpty())
        {
            for (auto& [adressIP, player] : m_Players)
            {
                if (opponentName != player) continue;

                m_GameServer->GetClientByName(adressIP)->Send(Message<OpponentLeftLobby>().Serialize().dump());
                break;
            }
        }

        break;
    }
    default:
        std::cout << WRN_CLR << "Received JSON from " << HASH_CLR(sender) << WRN_CLR << " contains an unknown type." << std::endl << DEF_CLR;
        break;
    }

    std::cout << std::endl;
}

void ServerApp::CleanUpGameServer()
{
    std::cout << Color::Cyan << "============== Starting Game Server Clean Up ==============" << std::endl;
    try
    {
        for (auto& c : m_GameServer->GetConnections())
        {
            c.Kick();
        }

        int count = 0;
        m_GameServer->CleanClosedConnections([&](ClientPtr c) { ++count; });
        if (count > 0)
            std::cout << INF_CLR << "Closed " << count << " connection" << (count > 1 ? "s" : "") << "." << std::endl;

        for (auto& [id, lobby] : m_StartedGames)
        {
            NULLPTR(lobby);
        }
        if (!m_StartedGames.empty())
            std::cout << INF_CLR << "Ended " << m_StartedGames.size() << " started game" << (m_StartedGames.size() > 1 ? "s" : "") << "." << std::endl;
        m_StartedGames.clear();

        for (auto lb : m_Lobbies)
        {
            RELEASE(lb);
        }
        if (!m_Lobbies.empty())
            std::cout << INF_CLR << "Deleted " << m_Lobbies.size() << " lobb" << (m_Lobbies.size() > 1 ? "ies" : "y") << "." << std::endl;
        m_Lobbies.clear();

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
        for (auto& c : m_WebServer->GetHtmlConns())
        {
            c.Kick();
        }

        int count = 0;
        m_WebServer->CleanClosedHtmlConns([&](WebClientPtr c) { ++count; });

        if (count > 0)
            std::cout << WEB_PFX << "Closed " << count << " connection" << (count > 1 ? "s" : "") << "." << std::endl;

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

bool ServerApp::IsPlayerInLobby(const std::string& name) const
{
    for (const auto lobby : m_Lobbies)
    {
        if (lobby->IsInLobby(name))
            return true;
    }
    return false;
}

void ServerApp::CreateLobbies()
{
    for (int i = 0; i < MAXIMUM_LOBBIES; i++)
    {
        if (i <= 2 )
        {
            m_Lobbies.emplace_back(new Lobby());
        }
        else
        {
            m_Lobbies.emplace_back(new Lobby(FAST));
        }
    }
}

#pragma endregion
