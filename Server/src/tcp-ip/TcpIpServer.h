#pragma once
#include <WinSock2.h>

typedef SOCKET Client;

/// <summary>
/// TCP/IP server.
/// </summary>
class TcpIpServer final
{
    TcpIpServer();
    ~TcpIpServer();
    TcpIpServer(const TcpIpServer&) = delete;
    TcpIpServer& operator=(const TcpIpServer&) = delete;

public:
    static TcpIpServer& GetInstance()
    {
        static TcpIpServer instance;
        return instance;
    }

    /// <summary>
    /// Open server socket.
    /// </summary>
    /// <param name="port">Port number to listen.</param>
    void Open(int port);
    /// <summary>
    /// Start listening.
    /// </summary>
    void StartListening();
    /// <summary>
    /// Accept the next connection request and return client socket. (Blocking)
    /// </summary>
    Client WaitForClient();
    /// <summary>
    /// Receive data from client. (Blocking, until client close connection)
    /// </summary>
    void Receive(const Client& client, std::stringstream& ss, const unsigned int bufferSize = 512);
    /// <summary>
    /// Send data to client.
    /// </summary>
    void Send(const Client& client, const std::string& data);
    /// <summary>
    /// Disconnect a client.
    /// </summary>
    void Disconnect(Client& client);
    /// <summary>
    /// Close server socket.
    /// </summary>
    void Close();

private:

    WSADATA m_WsaData;
    SOCKET m_ListenSocket;
};
