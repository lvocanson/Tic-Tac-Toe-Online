#include "TcpIp.h"

#if defined(DEBUG) | defined(_DEBUG)
#include <crtdbg.h>
// Dynamically allocate memory with memory leak detection. (debug only)
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

namespace TcpIp
{
    WSADATA InitializeWinsock()
    {
        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0)
            throw TcpIpException("WSAStartup", iResult);
        return wsaData;
    }

    void CleanupWinsock()
    {
        int iResult = WSACleanup();
        if (iResult != 0)
            throw TcpIpException("WSACleanup", iResult);
    }

    std::string GetErrMsg(const char* operation)
    {
        std::stringstream ss;
        ss << operation << " failed with error: " << WSAGetLastError();
        return ss.str();
    }

    std::string GetErrMsg(const char* operation, int resultCode)
    {
        std::stringstream ss;
        ss << operation << " failed with error: " << resultCode;
        return ss.str();
    }

    TcpIpException::TcpIpException(const char* operation)
        : std::runtime_error(GetErrMsg(operation))
    {
    }

    TcpIpException::TcpIpException(const char* operation, int resultCode)
        : std::runtime_error(GetErrMsg(operation, resultCode))
    {
    }

    constexpr const char* const HEADER_SIGNATURE = "T1cT4cT0z";
    constexpr const int HEADER_SIGNATURE_SIZE = 9;
    constexpr const int HEADER_SIZE = HEADER_SIGNATURE_SIZE + sizeof(size_t);

    char* CreateHeader(std::string& data)
    {
        char* header = new char[HEADER_SIZE];
        memcpy(header, HEADER_SIGNATURE, HEADER_SIGNATURE_SIZE);

        size_t size = data.size();
        memcpy(header + HEADER_SIGNATURE_SIZE, &size, sizeof(size_t));

        return header;
    }

    bool IsHeaderValid(char* header)
    {
        return memcmp(header, HEADER_SIGNATURE, HEADER_SIGNATURE_SIZE) == 0;
    }

    size_t GetSizeFromHeader(char* header)
    {
        size_t size;
        memcpy(&size, header + HEADER_SIGNATURE_SIZE, sizeof(size_t));
        return size;
    }

    void Send(const SOCKET& socket, const std::string& data)
    {
        // Send header
        char* header = CreateHeader(const_cast<std::string&>(data));
        int iResult = send(socket, header, HEADER_SIZE, 0);
        delete[] header;
        if (iResult == SOCKET_ERROR)
            throw TcpIpException("send");

        // Send data
        iResult = send(socket, data.c_str(), static_cast<int>(data.size()), 0);
        if (iResult == SOCKET_ERROR)
            throw TcpIpException("send");
    }

    void Receive(const SOCKET& socket, std::stringstream& ss, const unsigned int bufferSize)
    {
        // Receive header
        char* header = new char[HEADER_SIZE];
        int iResult = recv(socket, header, HEADER_SIZE, 0);
        if (iResult == SOCKET_ERROR)
            throw TcpIpException("recv");

        // Check header
        if (!IsHeaderValid(header))
            throw std::runtime_error("Received invalid header.");

        // Receive data
        size_t size = GetSizeFromHeader(header);
        delete[] header;
        char* buffer = new char[bufferSize];

        while (size > 0)
        {
            iResult = recv(socket, buffer, bufferSize, 0);
            if (iResult == SOCKET_ERROR)
                throw TcpIpException("recv");

            ss.write(buffer, iResult);
            size -= iResult;
        }
        delete[] buffer;
    }

    void CloseSocket(SOCKET& socket)
    {
        if (closesocket(socket) == SOCKET_ERROR)
            throw TcpIpException("closesocket");
        socket = INVALID_SOCKET;
    }

    WSAEVENT CreateEventObject(const SOCKET& socket, const long networkEvents)
    {
        WSAEVENT event = WSACreateEvent();
        if (event == WSA_INVALID_EVENT)
            throw TcpIpException("WSACreateEvent");

        int iResult = WSAEventSelect(socket, event, networkEvents);
        if (iResult == SOCKET_ERROR)
            throw TcpIpException("WSAEventSelect", iResult);

        return event;
    }

    void CloseEventObject(WSAEVENT& event)
    {
        if (WSACloseEvent(event) == FALSE)
            throw TcpIpException("WSACloseEvent");
        event = WSA_INVALID_EVENT;
    }
}