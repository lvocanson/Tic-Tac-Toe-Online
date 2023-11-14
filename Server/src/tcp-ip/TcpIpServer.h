#pragma once
#include <WinSock2.h>
#include <vector>

using Client = SOCKET;

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
    /// Open server and start listening.
    /// </summary>
    /// <param name="port">Port number to listen.</param>
    void Open(unsigned int port);
    /// <summary>
    /// Stop listening and close all connections.
    /// </summary>
    void Close();

    /// <summary>
    /// Accepts pending connections.
    /// </summary>
    /// <returns>True if a connection was accepted, false otherwise.</returns>
    bool AcceptPendingConnections();
    /// <summary>
    /// Fetches pending data from the first client that has data to read.
    /// </summary>
    /// <param name="ss">The stringstream to write the data to.</param>
    /// <param name="client">The client to respond to.</param>
    /// <returns>True if data was fetched, false otherwise.</returns>
    bool FetchPendingData(std::stringstream& ss, Client& client);
    /// <summary>
    /// Sends data to a client.
    /// </summary>
    void Send(const Client& Client, const std::string& data);
    /// <summary>
    /// Kills all closed connections. Call this function after FetchPendingData.
    /// </summary>
    /// <returns>The number of connections killed.</returns>
    unsigned int KillClosedConnections();

private:
    struct Connection
    {
        // Creates the event object and associate it with the socket.
        Connection(SOCKET socket);
        SOCKET Socket;
        WSAEVENT Event;
    };

    WSADATA m_WsaData;
    SOCKET m_ListenSocket;
    WSAEVENT m_AcceptEvent;

    std::vector<Connection> m_Connections;
};
