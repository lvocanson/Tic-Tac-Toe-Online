#pragma once
#include <WinSock2.h>
#include <vector>

/// <summary>
/// A connection to a client.
/// </summary>
struct HtmlConn
{
    std::string Address = "Unknown";
    unsigned int Port = 0;
    std::string GetName() const;

    std::string Receive() const;
    void Send(const std::string& data) const;
    void Kick() const;

    // Creates the event object and associate it with the socket.
    HtmlConn(SOCKET socket);
private:
    friend class HtmlServer;

    SOCKET Socket;

    bool IsNew = true;
    mutable bool ReadPending = false;
    mutable bool ClosePending = false;
};
/// <summary>
/// A pointer to a connection.
/// Do not store this pointer, it may become invalid.
/// </summary>
typedef HtmlConn* WebClientPtr;

class HtmlServer final
{
public:
    HtmlServer();
    ~HtmlServer();
    HtmlServer(const HtmlServer&) = delete;
    HtmlServer& operator=(const HtmlServer&) = delete;

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
    /// Accept new connections and check all clients for pending data and close requests.
    /// </summary>
    void CheckNetwork();
    /// <summary>
    /// Custom WNDPROC for the server.
    /// </summary>
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    /// <summary>
    /// Find a client that has just connected.
    /// </summary>
    /// <returns>A client that is new, or nullptr.</returns>
    WebClientPtr FindNewClient();
    /// <summary>
    /// Find a client that has pending data.
    /// </summary>
    /// <returns>A client that has pending data, or nullptr.</returns>
    WebClientPtr FindClientWithPendingData();
    /// <summary>
    /// Close all clients that are marked for closing.
    /// </summary>
    /// <param name="lastCallback">A callback that will be called for each client that is closed.</param>
    /// <returns>The number of connections that were closed.</returns>
    int CleanClosedHtmlConns(std::function<void(WebClientPtr)> lastCallback = nullptr);

    /// <summary>
    /// Get all connections.
    /// </summary>
    const std::vector<HtmlConn>& GetHtmlConns() { return m_HtmlConns; }

private:
    WSADATA m_WsaData;
    SOCKET m_ListenSocket;

    std::vector<HtmlConn> m_HtmlConns;
};
