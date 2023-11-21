#pragma once
#include "src/tcp-ip/TcpIpServer.h"
#include <src/tcp-ip/HtmlServer.h>
#include "game/Lobby.h"

class ServerApp
{
public:
    ServerApp() = default;
    ~ServerApp() = default;
    ServerApp(const ServerApp&) = delete;
    ServerApp& operator=(const ServerApp&) = delete;

    void Init();
    void Run();
    void CleanUp();

private: // Game Server
    bool InitGameServer();
    void HandleGameServer();
    void HandleRecv(ClientPtr sender);
    void CleanUpGameServer();

    TcpIpServer* m_GameServer = nullptr;

private: // Web Server
    bool InitWebServer();
    void HandleWebServer();
    void HandleWebConnection();
    void CleanUpWebServer();

    HtmlServer* m_WebServer = nullptr;

private: // Lobbies
    size_t FindPlayer(const std::string& name);
    void CreateLobbies();
    void SerializeLobbiesToJson(ClientPtr sender);

    // HashMap <Username, Address (connection name)>
    std::unordered_map<std::string, std::string> m_Players;
    std::vector<Lobby> m_Lobbies;
};
