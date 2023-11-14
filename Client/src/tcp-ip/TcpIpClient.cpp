#include "TcpIpClient.h"

TcpIpClient::TcpIpClient()
    : m_WsaData(TcpIp::InitializeWinsock()), m_ConnectSocket(INVALID_SOCKET)
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
        TcpIp::ThrowWithResultCode("getaddrinfo", iResult);

    // Create a SOCKET for connecting to server 
    m_ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (m_ConnectSocket == INVALID_SOCKET)
    {
        freeaddrinfo(result);
        TcpIp::ThrowWithLastError("socket");
    }

    // Connect to server.
    iResult = connect(m_ConnectSocket, result->ai_addr, (int)result->ai_addrlen);
    freeaddrinfo(result);

    if (iResult == SOCKET_ERROR)
        Disconnect();
    if (m_ConnectSocket == INVALID_SOCKET)
        TcpIp::ThrowWithLastError("connect");
}

void TcpIpClient::Send(const std::string& data)
{
    if (m_ConnectSocket == INVALID_SOCKET)
        throw std::runtime_error("Not connected to a server.");

    TcpIp::Send(m_ConnectSocket, data);
}

void TcpIpClient::Receive(std::stringstream& ss, const unsigned int bufferSize)
{
    if (m_ConnectSocket == INVALID_SOCKET)
        throw std::runtime_error("Not connected to a server.");

    TcpIp::Receive(m_ConnectSocket, ss, bufferSize);
}

void TcpIpClient::Disconnect()
{
    if (m_ConnectSocket == INVALID_SOCKET)
        throw std::runtime_error("Not connected to a server."); 

    int iResult = shutdown(m_ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
        TcpIp::ThrowWithLastError("shutdown");

    TcpIp::CloseSocket(m_ConnectSocket);
}
