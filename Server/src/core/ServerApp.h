#pragma once
#include "src/tcp-ip/TcpIpServer.h"

class TcpIpServer;

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

private:

    bool InitGameServer();
    void HandleGameServer();
    void HandleData(const std::string& data, Client sender);
    void CleanUpGameServer();

    bool InitWebServer();
    void HandleWebServer();
    void CleanUpWebServer();

    TcpIpServer* m_GameServer = nullptr;
};
