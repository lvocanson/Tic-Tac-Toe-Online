#pragma once
#include <WinSock2.h>

/// <summary>
/// TCP/IP Client.
/// </summary>
class TcpIpClient final
{
    TcpIpClient();
    ~TcpIpClient();
    TcpIpClient(const TcpIpClient&) = delete;
    TcpIpClient& operator=(const TcpIpClient&) = delete;

public:
    static TcpIpClient& GetInstance()
    {
        static TcpIpClient instance;
        return instance;
    }

    /// <summary>
    /// Connects to a server.
    /// </summary>
    void Connect(const char* ip, int port);
    /// <summary>
    /// Sends data to the connected server.
    /// </summary>
    void Send(const std::string& data);
    /// <summary>
    /// Receives data from the connected server. (Blocking, until server closes connection)
    /// </summary>
    void Receive(std::stringstream& ss, const unsigned int bufferSize = 512);
    /// <summary>
    /// Disconnects from the server.
    /// </summary>
    void Disconnect();

private:

    WSADATA m_WsaData;
    SOCKET m_ConnectSocket;
};
