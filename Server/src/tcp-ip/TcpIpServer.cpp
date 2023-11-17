#include "TcpIpServer.h"
using enum TcpIp::ErrorCode;

std::string Connection::Receive()
{
    if (!ReadPending)
        throw TcpIp::TcpIpException::Create(SOCKET_NoDataAvailable);

    std::stringstream ss;
    TcpIp::Receive(Socket, ss, DEFAULT_BUFFER_SIZE);
    ReadPending = false;
    return ss.str();
}

void Connection::Send(const std::string& data)
{
    TcpIp::Send(Socket, data.c_str(), static_cast<u_long>(data.size()));
}

Connection::Connection(SOCKET socket)
    : Socket(socket)
    , Event(TcpIp::CreateEventObject(socket, FD_READ | FD_CLOSE))
{
    sockaddr_in clientAddress;
    int clientAddressLength = sizeof(clientAddress);
    if (getpeername(socket, (sockaddr*)&clientAddress, &clientAddressLength) == 0)
    {
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddress.sin_addr), clientIP, INET_ADDRSTRLEN);
        Address = clientIP;
        Port = ntohs(clientAddress.sin_port);
    }
}

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
    {
        freeaddrinfo(result);
        throw TcpIp::TcpIpException::Create(WSA_ResolveFailed, iResult);
    }

    // Create listening socket
    m_ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (m_ListenSocket == INVALID_SOCKET)
    {
        freeaddrinfo(result);
        throw TcpIp::TcpIpException::Create(SOCKET_CreateFailed, TCP_IP_WSA_ERROR);
    }

    // Setup the TCP listening socket
    iResult = bind(m_ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    freeaddrinfo(result);

    if (iResult == SOCKET_ERROR)
        throw TcpIp::TcpIpException::Create(SOCKET_BindFailed, TCP_IP_WSA_ERROR);

    // Start listening on the socket
    if (listen(m_ListenSocket, SOMAXCONN) == SOCKET_ERROR)
        throw TcpIp::TcpIpException::Create(SOCKET_ListenFailed, TCP_IP_WSA_ERROR);

    m_AcceptEvent = TcpIp::CreateEventObject(m_ListenSocket, FD_ACCEPT);
}

void TcpIpServer::Close()
{
    if (m_ListenSocket != INVALID_SOCKET)
    {
        TcpIp::CloseEventObject(m_AcceptEvent);
        TcpIp::CloseSocket(m_ListenSocket);
    }

    for (Connection& connection : m_Connections)
    {
        if (connection.Socket == INVALID_SOCKET)
            continue; // Already closed

        TcpIp::CloseEventObject(connection.Event);
        TcpIp::CloseSocket(connection.Socket);
    }
    m_Connections.clear();
}

ClientPtr TcpIpServer::AcceptPendingConnection()
{
    // Check the listening socket for accept events
    WSANETWORKEVENTS networkEvents;
    int iResult = WSAEnumNetworkEvents(m_ListenSocket, m_AcceptEvent, &networkEvents);
    if (iResult == SOCKET_ERROR)
        throw TcpIp::TcpIpException::Create(EVENT_EnumFailed, TCP_IP_WSA_ERROR);

    if (networkEvents.lNetworkEvents & FD_ACCEPT)
    {
        if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
            throw TcpIp::TcpIpException::Create(EVENT_FdAcceptHadError, networkEvents.iErrorCode[FD_ACCEPT_BIT]);

        SOCKET connectionSocket = accept(m_ListenSocket, nullptr, nullptr);
        if (connectionSocket == INVALID_SOCKET)
            throw TcpIp::TcpIpException::Create(SOCKET_CreateFailed, TCP_IP_WSA_ERROR);

        // Create a new connection, and place it at the end of the vector
        m_Connections.emplace_back(connectionSocket);
        return &m_Connections.back();
    }
    return nullptr;
}

std::vector<ClientPtr> TcpIpServer::AcceptAllPendingConnections()
{
    std::vector<ClientPtr> clients;
    ClientPtr client = nullptr;
    while ((client = AcceptPendingConnection()) != nullptr)
        clients.push_back(client);
    return clients;
}

void TcpIpServer::Kick(ClientPtr& client)
{
    client->ClosePending = true;
}

void TcpIpServer::CheckNetwork()
{
    WSANETWORKEVENTS networkEvents;
    for (Connection& connection : m_Connections)
    {

        // Enumerate the network events for this connection
        int iResult = WSAEnumNetworkEvents(connection.Socket, connection.Event, &networkEvents);
        if (iResult == SOCKET_ERROR)
            throw TcpIp::TcpIpException::Create(EVENT_EnumFailed, TCP_IP_WSA_ERROR);

        if (networkEvents.lNetworkEvents & FD_READ)
        {
            if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
                throw TcpIp::TcpIpException::Create(EVENT_FdReadHadError, networkEvents.iErrorCode[FD_READ_BIT]);

            connection.ReadPending = true;
        }

        if (networkEvents.lNetworkEvents & FD_CLOSE)
        {
            if (networkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
                throw TcpIp::TcpIpException::Create(EVENT_FdCloseHadError, networkEvents.iErrorCode[FD_CLOSE_BIT]);

            connection.ClosePending = true;
        }
    }
}

ClientPtr TcpIpServer::FindClientWithPendingData()
{
    for (Connection& connection : m_Connections)
    {
        if (connection.ReadPending)
            return &connection;
    }
    return nullptr;
}

size_t TcpIpServer::CleanClosedConnections()
{
    // Move all connections that have a close pending to the end of the vector
    auto partition = std::partition(m_Connections.begin(), m_Connections.end(),
        [](const Connection& connection) { return !connection.ClosePending; });

    // Close them all & remove them from the vector
    for (auto it = partition; it != m_Connections.end(); ++it)
    {
        TcpIp::CloseEventObject(it->Event);
        TcpIp::CloseSocket(it->Socket);
    }

    size_t closedConnections = std::distance(partition, m_Connections.end());
    m_Connections.erase(partition, m_Connections.end());
    return closedConnections;
}
