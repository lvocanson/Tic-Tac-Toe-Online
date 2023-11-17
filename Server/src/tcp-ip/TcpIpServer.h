#pragma once
#include <WinSock2.h>
#include <vector>

/// <summary>
/// A connection to a client.
/// </summary>
struct Connection
{
    std::string Address = "Unknown";
    unsigned int Port = 0;

    std::string Receive();
    void Send(const std::string& data);

    // Creates the event object and associate it with the socket.
    Connection(SOCKET socket);
private:
    friend class TcpIpServer;

    SOCKET Socket;
    WSAEVENT Event;

    bool ReadPending = false;
    bool ClosePending = false;
};
/// <summary>
/// A pointer to a connection.
/// Do not store this pointer, it may become invalid.
/// </summary>
typedef Connection* ClientPtr;

/// <summary>
/// TCP/IP server.
/// </summary>
class TcpIpServer final
{
public:
    TcpIpServer();
    ~TcpIpServer();
    TcpIpServer(const TcpIpServer&) = delete;
    TcpIpServer& operator=(const TcpIpServer&) = delete;

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
    /// Accept the first pending connection, if any.
    /// </summary>
    /// <returns>The client that connected, or nullptr.</returns>
    ClientPtr AcceptPendingConnection();
    /// <summary>
    /// Accept all pending connections.
    /// </summary>
    /// <returns>The clients that connected.</returns>
    std::vector<ClientPtr> AcceptAllPendingConnections();
    /// <summary>
    /// Mark a client for closing.
    /// </summary>
    void Kick(ClientPtr& ClientPtr);

    /// <summary>
    /// Check all clients for pending data and close requests.
    /// </summary>
    void CheckNetwork();
    /// <summary>
    /// Find a client that has pending data.
    /// </summary>
    /// <returns>A client that has pending data, or nullptr.</returns>
    ClientPtr FindClientWithPendingData();
    /// <summary>
    /// Close all clients that are marked for closing.
    /// </summary>
    /// <returns>The number of connections that were closed.</returns>
    size_t CleanClosedConnections();

private:

    WSADATA m_WsaData;
    SOCKET m_ListenSocket;
    WSAEVENT m_AcceptEvent;

    std::vector<Connection> m_Connections;
};
