#include "TcpIpServer.h"

TcpIpServer::TcpIpServer()
    : m_WsaData(TcpIp::InitializeWinsock()), m_ListenSocket(INVALID_SOCKET)
{
}

TcpIpServer::~TcpIpServer()
{
    if (m_ListenSocket != INVALID_SOCKET)
        Close();
    TcpIp::CleanupWinsock();
}

void TcpIpServer::Open(int port)
{
    ADDRINFO* result = nullptr;
    ADDRINFO hints
    {
        .ai_flags = AI_PASSIVE,
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = IPPROTO_TCP,
    };

    // Resolve the server address and port
    int iResult = getaddrinfo(nullptr, std::to_string(port).c_str(), &hints, &result);
    if (iResult != 0)
        TcpIp::ThrowWithResultCode("getaddrinfo", iResult);

    // Create listening socket
    m_ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (m_ListenSocket == INVALID_SOCKET)
    {
        freeaddrinfo(result);
        TcpIp::ThrowWithLastError("socket");
    }

    // Setup the TCP listening socket
    iResult = bind(m_ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    freeaddrinfo(result);

    if (iResult == SOCKET_ERROR)
        TcpIp::ThrowWithLastError("bind");
}

void TcpIpServer::StartListening()
{
    if (listen(m_ListenSocket, SOMAXCONN) == SOCKET_ERROR)
        TcpIp::ThrowWithLastError("listen");
}

Client TcpIpServer::WaitForClient()
{
    SOCKET clientSocket = accept(m_ListenSocket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET)
        TcpIp::ThrowWithLastError("accept");

    return clientSocket;
}

void TcpIpServer::Receive(const Client& client, std::stringstream& ss, const unsigned int bufferSize)
{
    TcpIp::Receive(client, ss, bufferSize);
}

void TcpIpServer::Send(const Client& client, const std::string& data)
{
    TcpIp::Send(client, data);
}

void TcpIpServer::Disconnect(Client& client)
{
    int iResult = shutdown(client, SD_SEND);
    if (iResult == SOCKET_ERROR)
        TcpIp::ThrowWithLastError("shutdown");

    TcpIp::CloseSocket(client);
}

void TcpIpServer::Close()
{
    TcpIp::CloseSocket(m_ListenSocket);
}
