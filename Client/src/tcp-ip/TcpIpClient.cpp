#include "TcpIpClient.h"

TcpIpClient::TcpIpClient()
    : m_WsaData(TcpIp::InitializeWinsock())
    , m_ConnectSocket(INVALID_SOCKET)
    , m_ReadEvent(WSA_INVALID_EVENT)
{
}

TcpIpClient::~TcpIpClient()
{
    if (m_ConnectSocket != INVALID_SOCKET)
        Disconnect();
    TcpIp::CleanupWinsock();
}

void TcpIpClient::Connect(const char* ip, int port)
{
    m_ConnectSocket = INVALID_SOCKET;
    ADDRINFO* result = nullptr;
    addrinfo hints = {};
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    int iResult = getaddrinfo(ip, std::to_string(port).c_str(), &hints, &result);
    if (iResult != 0)
    {
        freeaddrinfo(result);
        throw TcpIp::TcpIpException("getaddrinfo", iResult);
    }

    // Create a SOCKET for connecting to server 
    m_ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (m_ConnectSocket == INVALID_SOCKET)
    {
        freeaddrinfo(result);
        throw TcpIp::TcpIpException("socket", TCP_IP_WSA_ERROR);
    }

    // Connect to server.
    iResult = connect(m_ConnectSocket, result->ai_addr, (int)result->ai_addrlen);
    freeaddrinfo(result);

    if (iResult == SOCKET_ERROR)
        TcpIp::CloseSocket(m_ConnectSocket);
    if (m_ConnectSocket == INVALID_SOCKET)
        throw TcpIp::TcpIpException("connect", TCP_IP_WSA_ERROR);

    m_ReadEvent = TcpIp::CreateEventObject(m_ConnectSocket, FD_READ | FD_CLOSE);
}

void TcpIpClient::Disconnect()
{
    if (m_ConnectSocket == INVALID_SOCKET)
        return;

    int iResult = shutdown(m_ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
        throw TcpIp::TcpIpException("shutdown", TCP_IP_WSA_ERROR);

    TcpIp::CloseEventObject(m_ReadEvent);
    TcpIp::CloseSocket(m_ConnectSocket);
}

bool TcpIpClient::IsConnected() const
{
    return m_ConnectSocket != INVALID_SOCKET;
}

void TcpIpClient::Send(const char* data, u_long size)
{
    TcpIp::Send(m_ConnectSocket, data, size);
}

bool TcpIpClient::FetchPendingData(std::stringstream& ss)
{
    WSANETWORKEVENTS networkEvents;
    int iResult = WSAEnumNetworkEvents(m_ConnectSocket, m_ReadEvent, &networkEvents);
    if (iResult == SOCKET_ERROR)
        throw TcpIp::TcpIpException("WSAEnumNetworkEvents", TCP_IP_WSA_ERROR);

    if (networkEvents.lNetworkEvents & FD_READ)
    {
        TcpIp::Receive(m_ConnectSocket, ss, DEFAULT_BUFFER_SIZE);
        return true;
    }
    if (networkEvents.lNetworkEvents & FD_CLOSE)
    {
        Disconnect();
    }
    return false;
}
