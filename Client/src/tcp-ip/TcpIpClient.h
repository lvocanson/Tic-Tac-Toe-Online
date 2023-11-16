#pragma once
#include <WinSock2.h>

/// <summary>
/// TCP/IP Client.
/// </summary>
class TcpIpClient final
{
public:
    TcpIpClient();
    ~TcpIpClient();
    TcpIpClient(const TcpIpClient&) = delete;
    TcpIpClient& operator=(const TcpIpClient&) = delete;

    /// <summary>
    /// Connects to a server.
    /// </summary>
    void Connect(const char* ip, int port);
    /// <summary>
    /// Disconnects from the server.
    /// </summary>
    void Disconnect();
    /// <summary>
    /// Checks if the client is connected to a server.
    /// </summary>
    bool IsConnected() const;

    /// <summary>
    /// Sends data to the connected server.
    /// </summary>
    void Send(const char* data, u_long size);
    void Send(const std::string& data) { Send(data.c_str(), static_cast<u_long>(data.size())); }
    /// <summary>
    /// Fetches pending data from the server.
    /// </summary>
    /// <param name="ss">The stringstream to write the data to.</param>
    /// <returns>True if data was fetched, false otherwise.</returns>
    bool FetchPendingData(std::stringstream& ss);

private:

    WSADATA m_WsaData;
    SOCKET m_ConnectSocket;
    WSAEVENT m_ReadEvent;
};
