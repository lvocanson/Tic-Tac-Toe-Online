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

    void Run();
    void AnalyseData(const std::string& data, Client sender); 

private:
    TcpIpServer* m_Server = nullptr;
};
