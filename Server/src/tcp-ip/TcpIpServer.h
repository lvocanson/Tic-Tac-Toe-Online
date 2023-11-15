#pragma once
#include <WinSock2.h>
#include <vector>

/// <summary>
/// A connection to a client.
/// </summary>
struct Connection
{
    std::string Address;
    unsigned int Port;

    // Creates the event object and associate it with the socket.
    Connection(SOCKET socket);
private:
    friend class TcpIpServer;

    SOCKET Socket;
    WSAEVENT Event;
};
typedef Connection* Client;

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
    /// Accept the first pending connection.
    /// </summary>
    /// <returns>True if a connection was accepted, false otherwise.</returns>
    bool AcceptPendingConnection();
    /// <summary>
    /// Accept all pending connections.
    /// </summary>
    /// <returns>The number of connections accepted.</returns>
    int AcceptAllPendingConnections();
    /// <summary>
    /// Fetches pending data from the first client that has data to read.
    /// If a client has closed the connection, the connection will be closed.
    /// </summary>
    /// <param name="ss">The stringstream to write the data to.</param>
    /// <param name="client">The client to respond to.</param>
    /// <returns>True if data was fetched, false otherwise.</returns>
    bool FetchPendingData(std::stringstream& ss, Client& client);
    /// <summary>
    /// Sends data to a client.
    /// </summary>
    void Send(const Client& Client, const char* data, u_long size);
    void Send(const Client& Client, const std::string& data) { Send(Client, data.c_str(), static_cast<u_long>(data.size())); }
    /// <summary>
    /// Kills all closed connections. Call this function after FetchPendingData.
    /// </summary>
    /// <returns>The number of connections killed.</returns>
    unsigned int KillClosedConnections();
    /// <summary>
    /// Get the number of connections.
    /// </summary>
    size_t ConnectionCount() const { return m_Connections.size(); }

private:

    WSADATA m_WsaData;
    SOCKET m_ListenSocket;
    WSAEVENT m_AcceptEvent;

    std::vector<Connection> m_Connections;
};
