#include "TcpIp.h"

#if defined(DEBUG) | defined(_DEBUG)
#include <crtdbg.h>
// Dynamically allocate memory with memory leak detection. (debug only)
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

namespace TcpIp
{
    /// <summary>
    /// Helper class to initialize and cleanup Winsock.
    /// </summary>
    struct WsaInit
    {
    public:
        static WsaInit& GetInstance()
        {
            static WsaInit instance;
            return instance;
        }
        WSADATA WsaData;

    private:
        WsaInit()
        {
            int iResult = WSAStartup(MAKEWORD(2, 2), &WsaData);
            if (iResult != 0)
                throw TcpIpException("WSAStartup", iResult);
        }

        ~WsaInit()
        {
            WSACleanup();
        }
    };

    WSADATA& InitializeWinsock()
    {
        return WsaInit::GetInstance().WsaData;
    }

    std::string GetErrMsg(const char* operation, int errCode)
    {
        if (errCode == TCP_IP_WSA_ERROR) errCode = WSAGetLastError();
        std::ostringstream oss;
        oss << operation << " failed with error: " << errCode;
        return oss.str();
    }

    TcpIpException::TcpIpException(const char* message)
        : std::runtime_error(message)
    {
    }

    TcpIpException::TcpIpException(const char* operation, int errCode)
        : std::runtime_error(GetErrMsg(operation, errCode))
    {
    }

#pragma region Header

    constexpr const char* const HEADER_SIGNATURE = "T1cT4cT0z";
    constexpr const int HEADER_SIGNATURE_SIZE = 9;
    constexpr const int HEADER_SIZE = HEADER_SIGNATURE_SIZE + sizeof(u_long);

    // Create a header to describe a data of the given size.
    // /!\ The caller is responsible for the pointer created!
    char* CreateHeader(const u_long size)
    {
        char* header = new char[HEADER_SIZE];
        memcpy(header, HEADER_SIGNATURE, HEADER_SIGNATURE_SIZE);

        u_long networkSize = htonl(size); // Convert to network byte order
        memcpy(header + HEADER_SIGNATURE_SIZE, &networkSize, sizeof(u_long));
        return header;
    }

    // Check if the given header has our signature.
    inline bool IsHeaderValid(char* header)
    {
        return memcmp(header, HEADER_SIGNATURE, HEADER_SIGNATURE_SIZE) == 0;
    }

    // Get the size of the next data in the given header.
    inline u_long GetSizeFromHeader(char* header)
    {
        u_long networkSize;
        memcpy(&networkSize, header + HEADER_SIGNATURE_SIZE, sizeof(u_long));
        return ntohl(networkSize); // Convert to host byte order
    }

#pragma endregion

    void Send(const SOCKET& socket, const char* data, const u_long size)
    {
        // Send header
        char* header = CreateHeader(size);
        int iResult = send(socket, header, HEADER_SIZE, 0);
        delete[] header;

        // iResult is number of bytes sent
        if (iResult == SOCKET_ERROR)
            throw TcpIpException("send (header)", TCP_IP_WSA_ERROR);

        // Send data
        iResult = send(socket, data, static_cast<int>(size), 0);
        if (iResult == SOCKET_ERROR)
            throw TcpIpException("send (data)", TCP_IP_WSA_ERROR);
    }

    void Receive(const SOCKET& socket, std::stringstream& ss, const unsigned int bufferSize)
    {
        // Receive header
        char* header = new char[HEADER_SIZE];
        int iResult = recv(socket, header, HEADER_SIZE, 0);

        // iResult is number of bytes received
        if (iResult == SOCKET_ERROR)
        {
            delete[] header;
            throw TcpIpException("recv (header)", TCP_IP_WSA_ERROR);
        }

        // Check header
        if (!IsHeaderValid(header))
        {
            delete[] header;
            throw TcpIpException("Received invalid header.");
        }

        // Get the the data size
        u_long dataSize = GetSizeFromHeader(header);
        delete[] header;

        // Get the data
        char* buffer = new char[bufferSize];
        while (dataSize > 0)
        {
            // recvSize = min(dataSize, bufferSize), because there might be more data to receive
            int recvSize = dataSize > bufferSize ? bufferSize : static_cast<int>(dataSize);
            iResult = recv(socket, buffer, recvSize, 0);

            // iResult is number of bytes received
            if (iResult == SOCKET_ERROR)
            {
                delete[] buffer;
                throw TcpIpException("recv (data)", TCP_IP_WSA_ERROR);
            }

            if (iResult != recvSize)
            {
                delete[] buffer;
                throw TcpIpException("Received data of unexpected size.");
            }

            // Write buffer to stream
            ss.write(buffer, iResult);
            dataSize -= iResult;
        }
        delete[] buffer;

        if (dataSize != 0)
        {
            throw TcpIpException("Received too much data.");
        }
    }

    void CloseSocket(SOCKET& socket)
    {
        if (closesocket(socket) == SOCKET_ERROR)
            throw TcpIpException("closesocket", TCP_IP_WSA_ERROR);
        socket = INVALID_SOCKET;
    }

    WSAEVENT CreateEventObject(const SOCKET& socket, const long networkEvents)
    {
        WSAEVENT eventObj = WSACreateEvent();
        if (eventObj == WSA_INVALID_EVENT)
            throw TcpIpException("WSACreateEvent", TCP_IP_WSA_ERROR);

        // Set wanted flags on the event
        int iResult = WSAEventSelect(socket, eventObj, networkEvents);
        if (iResult == SOCKET_ERROR)
        {
            CloseEventObject(eventObj);
            throw TcpIpException("WSAEventSelect", iResult);
        }

        return eventObj;
    }

    void CloseEventObject(WSAEVENT& event)
    {
        if (WSACloseEvent(event) == FALSE)
            throw TcpIpException("WSACloseEvent", TCP_IP_WSA_ERROR);
        event = WSA_INVALID_EVENT;
    }
}
