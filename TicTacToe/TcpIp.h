#pragma once

#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>

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
    /// Throws std::runtime_error with last Winsock error message.
    /// </summary>
    void ThrowWithLastError(const char* operation);
    /// <summary>
    /// Throws std::runtime_error with a Winsock result code.
    /// </summary>
    void ThrowWithResultCode(const char* operation, int resultCode);

    /// <summary>
    /// Sends data to a socket.
    /// </summary>
    void Send(const SOCKET& socket, const std::string& data);
    /// <summary>
    /// Receives data from a socket. (Blocking, until peer shuts down the connection)
    /// </summary>
    void Receive(const SOCKET& socket, std::stringstream& ss, const unsigned int bufferSize);
    /// <summary>
    /// Closes a socket.
    /// </summary>
    void CloseSocket(SOCKET& socket);
}
