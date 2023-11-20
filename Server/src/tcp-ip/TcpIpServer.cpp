#include "TcpIpServer.h"
using enum TcpIp::ErrorCode;

#ifdef NO_EVENTS
#define WM_SOCKET (WM_USER + 1)
#endif // NO_EVENTS

std::string Connection::GetName() const
{
    return Address + ":" + std::to_string(Port);
}

std::string Connection::Receive() const
{
    if (!ReadPending)
        throw TcpIp::TcpIpException::Create(SOCKET_NoDataAvailable);

    std::stringstream ss;
    TcpIp::Receive(Socket, ss, DEFAULT_BUFFER_SIZE);
    ReadPending = false;
    return ss.str();
}

void Connection::Send(const std::string& data) const
{
    TcpIp::Send(Socket, data.c_str(), static_cast<u_long>(data.size()));
}

void Connection::Kick()
{
    ClosePending = true;
}

Connection::Connection(SOCKET socket)
    : Socket(socket)
#ifndef NO_EVENTS
    , Event(TcpIp::CreateEventObject(socket, FD_READ | FD_CLOSE))
#endif // NO_EVENTS
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
#ifndef NO_EVENTS
    , m_AcceptEvent(WSA_INVALID_EVENT)
#endif // !NO_EVENTS
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

#ifdef NO_EVENTS
    // Create an invisible window for message processing
    const auto title = L"Server " + std::to_wstring(port);
    WNDCLASSEX wcex =
    {
        .cbSize = sizeof(WNDCLASSEX),
        .style = 0,
        .lpfnWndProc = [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT
        {
            return reinterpret_cast<TcpIpServer*>(GetWindowLongPtr(hWnd, GWLP_USERDATA))->WndProc(hWnd, message, wParam, lParam);
        },
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = GetModuleHandle(nullptr),
        .hIcon = nullptr,
        .hCursor = nullptr,
        .hbrBackground = nullptr,
        .lpszMenuName = nullptr,
        .lpszClassName = title.c_str(),
        .hIconSm = nullptr
    };
    RegisterClassEx(&wcex);
    HWND hWnd = CreateWindowEx(0, title.c_str(), title.c_str(), 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, GetModuleHandle(nullptr), nullptr);

    if (hWnd == nullptr)
        throw TcpIp::TcpIpException::Create(EVENT_CreateFailed, GetLastError());

    iResult = WSAAsyncSelect(m_ListenSocket, hWnd, WM_SOCKET, FD_ACCEPT | FD_READ | FD_CLOSE);
    if (iResult != 0)
        throw TcpIp::TcpIpException::Create(EVENT_SelectFailed, TCP_IP_WSA_ERROR);
#else
    m_AcceptEvent = TcpIp::CreateEventObject(m_ListenSocket, FD_ACCEPT);
#endif
}

void TcpIpServer::Close()
{
    if (m_ListenSocket != INVALID_SOCKET)
    {
#ifndef NO_EVENTS
        TcpIp::CloseEventObject(m_AcceptEvent);
#endif // NO_EVENTS
        TcpIp::CloseSocket(m_ListenSocket);
    }

    for (Connection& connection : m_Connections)
    {
        if (connection.Socket == INVALID_SOCKET)
            continue; // Already closed

#ifndef NO_EVENTS
        TcpIp::CloseEventObject(connection.Event);
#endif // NO_EVENTS
        TcpIp::CloseSocket(connection.Socket);
    }
    m_Connections.clear();
}

#ifdef NO_EVENTS

LRESULT TcpIpServer::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_SOCKET)
    {
        switch (WSAGETSELECTEVENT(lParam))
        {
        case FD_ACCEPT:
        {
            if (WSAGETSELECTERROR(lParam) != 0)
                throw TcpIp::TcpIpException::Create(EVENT_FdAcceptHadError, WSAGETSELECTERROR(lParam));

            SOCKET connectionSocket = accept(wParam, nullptr, nullptr);
            if (connectionSocket == INVALID_SOCKET)
                throw TcpIp::TcpIpException::Create(SOCKET_CreateFailed, TCP_IP_WSA_ERROR);

            // Create a new connection, and place it at the end of the vector
            int iResult = WSAAsyncSelect(connectionSocket, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
            if (iResult != 0)
                throw TcpIp::TcpIpException::Create(EVENT_SelectFailed, TCP_IP_WSA_ERROR);

            m_Connections.emplace_back(connectionSocket);
            break;
        }
        case FD_READ:
        {
            if (WSAGETSELECTERROR(lParam) != 0)
                throw TcpIp::TcpIpException::Create(EVENT_FdReadHadError, WSAGETSELECTERROR(lParam));

            for (Connection& connection : m_Connections)
            {
                if (connection.Socket == wParam)
                {
                    connection.ReadPending = true;
                    break;
                }
            }
            break;
        }
        case FD_CLOSE:
        {
            if (WSAGETSELECTERROR(lParam) != 0)
                throw TcpIp::TcpIpException::Create(EVENT_FdCloseHadError, WSAGETSELECTERROR(lParam));

            for (Connection& connection : m_Connections)
            {
                if (connection.Socket == wParam)
                {
                    connection.ClosePending = true;
                    break;
                }
            }
            break;
        }
        default: break;
        }
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

#endif // NO_EVENTS

void TcpIpServer::CheckNetwork()
{
#ifdef NO_EVENTS

    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

#else // NO_EVENTS

    WSANETWORKEVENTS networkEvents;

    // Check for accept events
    int iResult = WSAEnumNetworkEvents(m_ListenSocket, m_AcceptEvent, &networkEvents);
    if (iResult == SOCKET_ERROR)
        throw TcpIp::TcpIpException::Create(EVENT_EnumFailed, TCP_IP_WSA_ERROR);

    while (networkEvents.lNetworkEvents & FD_ACCEPT)
    {
        if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
            throw TcpIp::TcpIpException::Create(EVENT_FdAcceptHadError, networkEvents.iErrorCode[FD_ACCEPT_BIT]);

        SOCKET connectionSocket = accept(m_ListenSocket, nullptr, nullptr);
        if (connectionSocket == INVALID_SOCKET)
            throw TcpIp::TcpIpException::Create(SOCKET_CreateFailed, TCP_IP_WSA_ERROR);

        // Create a new connection, and place it at the end of the vector
        m_Connections.emplace_back(connectionSocket);

        int iResult = WSAEnumNetworkEvents(m_ListenSocket, m_AcceptEvent, &networkEvents);
        if (iResult == SOCKET_ERROR)
            throw TcpIp::TcpIpException::Create(EVENT_EnumFailed, TCP_IP_WSA_ERROR);
    }

    // Check for read/close events on all connections
    for (Connection& connection : m_Connections)
    {
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
#endif // NO_EVENTS
}

ClientPtr TcpIpServer::FindNewClient()
{
    for (Connection& connection : m_Connections)
    {
        if (connection.IsNew)
        {
            connection.IsNew = false;
            return &connection;
        }
    }
    return nullptr;
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

int TcpIpServer::CleanClosedConnections(std::function<void(ClientPtr)> lastCallback)
{
    // Move all connections that have a close pending to the end of the vector
    auto partition = std::partition(m_Connections.begin(), m_Connections.end(),
        [](const Connection& connection) { return !connection.ClosePending; });

    // Close them all & remove them from the vector
    for (auto it = partition; it != m_Connections.end(); ++it)
    {
        if (lastCallback != nullptr)
            lastCallback(&*it);
#ifndef NO_EVENTS
        TcpIp::CloseEventObject(it->Event);
#endif // NO_EVENTS
        TcpIp::CloseSocket(it->Socket);
    }

    int closedConnections = (int)std::distance(partition, m_Connections.end());
    m_Connections.erase(partition, m_Connections.end());
    return closedConnections;
}
