#include "TcpIpServer.h"

TcpIpServer::TcpIpServer()
    : m_WsaData(TcpIp::InitializeWinsock())
    , m_ListenSocket(INVALID_SOCKET)
    , m_AcceptEvent(WSA_INVALID_EVENT)
{
}

TcpIpServer::~TcpIpServer()
{
    if (m_ListenSocket != INVALID_SOCKET)
        Close();
    TcpIp::CleanupWinsock();
}

void TcpIpServer::Open(unsigned int port)
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
        throw TcpIp::TcpIpException("getaddrinfo", iResult);

    // Create listening socket
    m_ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (m_ListenSocket == INVALID_SOCKET)
    {
        freeaddrinfo(result);
        throw TcpIp::TcpIpException("socket");
    }

    // Setup the TCP listening socket
    iResult = bind(m_ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    freeaddrinfo(result);

    if (iResult == SOCKET_ERROR)
        throw TcpIp::TcpIpException("bind");

    // Start listening on the socket
    if (listen(m_ListenSocket, SOMAXCONN) == SOCKET_ERROR)
        throw TcpIp::TcpIpException("listen");

    m_AcceptEvent = TcpIp::CreateEventObject(m_ListenSocket, FD_ACCEPT);
}

void TcpIpServer::Close()
{
    if (m_ListenSocket != INVALID_SOCKET)
    {
        if (closesocket(m_ListenSocket) == SOCKET_ERROR)
            throw TcpIp::TcpIpException("closesocket");
        m_ListenSocket = INVALID_SOCKET;
    }

    KillClosedConnections(); // In case some connections are already closed
    for (Connection& connection : m_Connections)
    {
        TcpIp::CloseEventObject(connection.Event);
        TcpIp::CloseSocket(connection.Socket);
    }
    m_Connections.clear();
}

bool TcpIpServer::AcceptPendingConnections()
{
    // Check the listening socket for accept events
    WSANETWORKEVENTS networkEvents;
    int iResult = WSAEnumNetworkEvents(m_ListenSocket, m_AcceptEvent, &networkEvents);
    if (iResult == SOCKET_ERROR)
        throw TcpIp::TcpIpException("WSAEnumNetworkEvents");

    if (networkEvents.lNetworkEvents & FD_ACCEPT)
    {
        if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
            throw TcpIp::TcpIpException("FD_ACCEPT", networkEvents.iErrorCode[FD_ACCEPT_BIT]);

        SOCKET connectionSocket = accept(m_ListenSocket, nullptr, nullptr);
        if (connectionSocket == INVALID_SOCKET)
            throw TcpIp::TcpIpException("accept");

        m_Connections.emplace_back(connectionSocket);
        return true;
    }
    return false;
}

bool TcpIpServer::FetchPendingData(std::stringstream& ss, Client& client)
{
    WSANETWORKEVENTS networkEvents;
    for (Connection& connection : m_Connections)
    {
        // Check the connection socket for read and close events
        int iResult = WSAEnumNetworkEvents(connection.Socket, connection.Event, &networkEvents);
        if (iResult == SOCKET_ERROR)
            throw TcpIp::TcpIpException("WSAEnumNetworkEvents");

        if (networkEvents.lNetworkEvents & FD_READ)
        {
            if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
                throw TcpIp::TcpIpException("FD_READ", networkEvents.iErrorCode[FD_READ_BIT]);

            client = connection.Socket;
            TcpIp::Receive(connection.Socket, ss, DEFAULT_BUFFER_SIZE);
            return true;
        }
        if (networkEvents.lNetworkEvents & FD_CLOSE)
        {
            if (networkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
                throw TcpIp::TcpIpException("FD_CLOSE", networkEvents.iErrorCode[FD_CLOSE_BIT]);

            TcpIp::CloseEventObject(connection.Event);
            TcpIp::CloseSocket(connection.Socket);
        }
    }
    return false;
}

void TcpIpServer::Send(const Client& client, const char* data, size_t size)
{
    TcpIp::Send(client, data, size);
}

unsigned int TcpIpServer::KillClosedConnections()
{
    // Get all closed connections
    auto StillOpen = [](const Connection& connection)
        {
            return connection.Socket != INVALID_SOCKET;
        };
    auto it = std::partition(m_Connections.begin(), m_Connections.end(), StillOpen);
    unsigned int count = static_cast<unsigned int>(std::distance(it, m_Connections.end()));

    // Erase them from the vector
    m_Connections.erase(it, m_Connections.end());
    return count;
}

TcpIpServer::Connection::Connection(SOCKET socket)
    : Socket(socket)
    , Event(TcpIp::CreateEventObject(socket, FD_READ | FD_CLOSE))
{
}