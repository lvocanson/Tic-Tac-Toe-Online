#pragma once

#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>

constexpr unsigned int DEFAULT_PORT = 63064;
constexpr size_t DEFAULT_BUFFER_SIZE = 1024;
constexpr auto TCP_IP_WSA_ERROR = 0;

/// <summary>
/// Helper functions for TCP/IP.
/// </summary>
namespace TcpIp
{
    /// <summary>
    /// Initializes Winsock and returns WSADATA.
    /// </summary>
    WSADATA InitializeWinsock();
    /// <summary>
    /// Cleans up Winsock.
    /// </summary>
    void CleanupWinsock();

    /// <summary>
    /// A custom exception for TCP/IP.
    /// </summary>
    class TcpIpException : public std::runtime_error
    {
    public:
        /// <summary>
        /// Creates a TcpIpException with a custom message.
        /// </summary>
        TcpIpException(const char* message);
        /// <summary>
        /// Creates a TcpIpException with the specified error code.
        /// </summary>
        /// <param name="operation">The operation that failed.</param>
        /// <param name="errCode">The error code. TCP_IP_WSA_ERROR will get the last error from WSA.</param>
        TcpIpException(const char* operation, int errCode);
    };

    /// <summary>
    /// Sends data to a socket.
    /// </summary>
    void Send(const SOCKET& socket, const char* data, size_t size);
    /// <summary>
    /// Receives data from a socket. (Blocking, until peer shuts down the connection)
    /// </summary>
    void Receive(const SOCKET& socket, std::stringstream& ss, const unsigned int bufferSize);
    /// <summary>
    /// Closes a socket and sets it to INVALID_SOCKET.
    /// </summary>
    void CloseSocket(SOCKET& socket);

    /// <summary>
    /// Creates an event that listen to the specified network events on the specified socket.
    /// </summary>
    WSAEVENT CreateEventObject(const SOCKET& socket, const long networkEvents);
    /// <summary>
    /// Closes an event object and sets it to WSA_INVALID_EVENT.
    /// </summary>
    void CloseEventObject(WSAEVENT& event);
}
