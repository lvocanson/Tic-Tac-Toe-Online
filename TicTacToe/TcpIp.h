#pragma once

#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>

constexpr unsigned int DEFAULT_PORT = 63064;
constexpr size_t DEFAULT_BUFFER_SIZE = 1024;
constexpr int TCP_IP_WSA_ERROR = 0;

/// <summary>
/// Helper functions for TCP/IP.
/// </summary>
namespace TcpIp
{
    /// <summary>
    /// Initializes Winsock and returns WSADATA.
    /// </summary>
    WSADATA& InitializeWinsock();

    /// <summary>
    /// Sends data to a socket.
    /// </summary>
    void Send(const SOCKET& socket, const char* data, u_long size);
    /// <summary>
    /// Receives data from a socket. (Blocking, until peer shuts down the connection)
    /// </summary>
    void Receive(const SOCKET& socket, std::stringstream& ss, const unsigned int bufferSize);
    /// <summary>
    /// Closes a socket and sets it to INVALID_SOCKET.
    /// </summary>
    void CloseSocket(SOCKET& socket);

    /// <summary>
    /// Receives a HTTP request from a socket. (Same as normal Receive, but not checking the header)
    /// </summary>
    void ReceiveHtmlRequest(const SOCKET& socket, std::stringstream& ss, const unsigned int bufferSize);
    /// <summary>
    /// Sends a HTTP header & response to a socket. (Same as normal Send, but with HTTP header)
    /// </summary>
    void SendHtmlResponse(const SOCKET& socket, const char* data, u_long size);

    /// <summary>
    /// Creates an event that listen to the specified network events on the specified socket.
    /// </summary>
    WSAEVENT CreateEventObject(const SOCKET& socket, const long networkEvents);
    /// <summary>
    /// Closes an event object and sets it to WSA_INVALID_EVENT.
    /// </summary>
    void CloseEventObject(WSAEVENT& event);

    /// <summary>
    /// Error codes for TCP/IP.
    /// </summary>
    enum class ErrorCode : unsigned int
    {
        WSA_StartupFailed,
        WSA_CleanupFailed,
        WSA_ResolveFailed,
        SOCKET_CreateFailed,
        SOCKET_ConnectFailed,
        SOCKET_BindFailed,
        SOCKET_ListenFailed,
        SOCKET_AcceptFailed,
        SOCKET_NoDataAvailable,
        SOCKET_ShutdownFailed,
        SOCKET_CloseFailed,
        EVENT_CreateFailed,
        EVENT_SelectFailed,
        EVENT_EnumFailed,
        EVENT_FdAcceptHadError,
        EVENT_FdReadHadError,
        EVENT_FdCloseHadError,
        EVENT_CloseFailed,
        SEND_HeaderFailed,
        SEND_DataFailed,
        RECEIVE_HeaderFailed,
        RECEIVE_HeaderHadInvalidSize,
        RECEIVE_HeaderHadInvalidSignature,
        RECEIVE_DataFailed,
        RECEIVE_DataHadInvalidSize,

#ifdef WINDOW_EVENT
        WINDOW_CreateFailed,
#endif // WINDOW_EVENT
    };
    /// <summary>
    /// A custom exception for TCP/IP.
    /// </summary>
    class TcpIpException : public std::runtime_error
    {
        TcpIpException(const char* message, ErrorCode code, int context)
            : std::runtime_error(message), Code(code), Context(context) {}
    public:
        /// <summary>
        /// Creates a TcpIpException with the specified error code.
        /// </summary>
        static TcpIpException Create(ErrorCode code);
        /// <summary>
        /// Creates a TcpIpException with the specified error code and context.
        /// </summary>
        /// <param name="context">The context of the error. Pass TCP_IP_WSA_ERROR to get the last WSA error.</param>
        static TcpIpException Create(ErrorCode code, int context);

        const ErrorCode Code;
        const int Context;
    };
}
