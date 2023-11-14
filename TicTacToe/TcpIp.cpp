#include "TcpIp.h"

namespace TcpIp
{
    WSADATA InitializeWinsock()
    {
        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0)
            ThrowWithResultCode("WSAStartup", iResult);
        return wsaData;
    }

    void CleanupWinsock()
    {
        int iResult = WSACleanup();
        if (iResult != 0)
            ThrowWithResultCode("WSACleanup", iResult);
    }

    void ThrowWithLastError(const char* operation)
    {
        std::stringstream ss;
        ss << operation << " failed with error: " << WSAGetLastError();
        throw std::runtime_error(ss.str());
    }

    void ThrowWithResultCode(const char* operation, int resultCode)
    {
        std::stringstream ss;
        ss << operation << " failed with error: " << resultCode;
        throw std::runtime_error(ss.str());
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
            ThrowWithLastError("send");

        // Send data
        iResult = send(socket, data.c_str(), static_cast<int>(data.size()), 0);
        if (iResult == SOCKET_ERROR)
            ThrowWithLastError("send");
    }

    void Receive(const SOCKET& socket, std::stringstream& ss, const unsigned int bufferSize)
    {
        // Receive header
        char* header = new char[HEADER_SIZE];
        int iResult = recv(socket, header, HEADER_SIZE, 0);
        if (iResult == SOCKET_ERROR)
            ThrowWithLastError("recv");

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
                ThrowWithLastError("recv");

            ss.write(buffer, iResult);
            size -= iResult;
        }
        delete[] buffer;
    }

    void CloseSocket(SOCKET& socket)
    {
        if (closesocket(socket) == SOCKET_ERROR)
            ThrowWithLastError("closesocket");
        socket = INVALID_SOCKET;
    }
}