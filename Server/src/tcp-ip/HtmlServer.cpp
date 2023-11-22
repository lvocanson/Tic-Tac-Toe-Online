#include "HtmlServer.h"
using enum TcpIp::ErrorCode;

std::string HtmlConn::GetName() const
{
    return Address + ":" + std::to_string(Port);
}

std::string HtmlConn::Receive()
{
    if (!ReadPending)
        throw TcpIp::TcpIpException::Create(SOCKET_NoDataAvailable);

    std::stringstream ss;
    TcpIp::ReceiveHtmlRequest(Socket, ss, DEFAULT_BUFFER_SIZE);
    ReadPending = false;
    return ss.str();
}

void HtmlConn::Send(const std::string& data)
{
    TcpIp::SendHtmlResponse(Socket, data.c_str(), static_cast<u_long>(data.size()));
}

void HtmlConn::Kick()
{
    ClosePending = true;
}

HtmlConn::HtmlConn(SOCKET socket)
    : Socket(socket)
{
    sockaddr_in clientAddress = {0};
    int clientAddressLength = sizeof(clientAddress);
    if (getpeername(socket, (sockaddr*)&clientAddress, &clientAddressLength) == 0)
    {
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddress.sin_addr), clientIP, INET_ADDRSTRLEN);
        Address = clientIP;
        Port = ntohs(clientAddress.sin_port);
    }
}

// Custom message for the socket events
#define WM_SOCKET (WM_USER + 1)

HtmlServer::HtmlServer()
    : m_WsaData(TcpIp::InitializeWinsock())
    , m_ListenSocket(INVALID_SOCKET)
    , m_HtmlConns()
{
}

HtmlServer::~HtmlServer()
{
    Close();
}

void HtmlServer::Open(unsigned int port)
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

    // Create an invisible window for message processing
    const std::wstring title = L"HTML SERVER " + std::to_wstring(port);
    WNDCLASSEX wcex =
    {
        .cbSize = sizeof(WNDCLASSEX),
        .style = 0,
        .lpfnWndProc = WndProc,
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
    SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    iResult = WSAAsyncSelect(m_ListenSocket, hWnd, WM_SOCKET, FD_ACCEPT | FD_READ | FD_CLOSE);
    if (iResult != 0)
        throw TcpIp::TcpIpException::Create(EVENT_SelectFailed, TCP_IP_WSA_ERROR);
}

void HtmlServer::Close()
{
    if (m_ListenSocket != INVALID_SOCKET)
    {
        TcpIp::CloseSocket(m_ListenSocket);
    }

    for (HtmlConn& connection : m_HtmlConns)
    {
        if (connection.Socket == INVALID_SOCKET)
            continue; // Already closed

        TcpIp::CloseSocket(connection.Socket);
    }
    m_HtmlConns.clear();
}

void HtmlServer::CheckNetwork()
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT HtmlServer::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HtmlServer* server = reinterpret_cast<HtmlServer*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

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

            server->m_HtmlConns.emplace_back(connectionSocket);
            break;
        }
        case FD_READ:
        {
            if (WSAGETSELECTERROR(lParam) != 0)
                throw TcpIp::TcpIpException::Create(EVENT_FdReadHadError, WSAGETSELECTERROR(lParam));

            for (HtmlConn& connection : server->m_HtmlConns)
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

            for (HtmlConn& connection : server->m_HtmlConns)
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

WebClientPtr HtmlServer::FindNewClient()
{
    for (HtmlConn& connection : m_HtmlConns)
    {
        if (connection.IsNew)
        {
            connection.IsNew = false;
            return &connection;
        }
    }
    return nullptr;
}

WebClientPtr HtmlServer::FindClientWithPendingData()
{
    for (HtmlConn& connection : m_HtmlConns)
    {
        if (connection.ReadPending && !connection.ClosePending)
            return &connection;
    }
    return nullptr;
}

int HtmlServer::CleanClosedHtmlConns(std::function<void(WebClientPtr)> lastCallback)
{
    // Move all connections that have a close pending to the end of the vector
    auto partition = std::partition(m_HtmlConns.begin(), m_HtmlConns.end(),
        [](const HtmlConn& connection) { return !connection.ClosePending; });

    // Close them all & remove them from the vector
    for (auto it = partition; it != m_HtmlConns.end(); ++it)
    {
        if (lastCallback != nullptr)
            lastCallback(&*it);
        TcpIp::CloseSocket(it->Socket);
    }

    int closedHtmlConns = (int)std::distance(partition, m_HtmlConns.end());
    m_HtmlConns.erase(partition, m_HtmlConns.end());
    return closedHtmlConns;
}
