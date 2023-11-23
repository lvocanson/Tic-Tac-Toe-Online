#include "TcpIp.h"

#if defined(DEBUG) | defined(_DEBUG)
#include <crtdbg.h>
// Dynamically allocate memory with memory leak detection. (debug only)
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

namespace TcpIp
{
    using enum ErrorCode;

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
                throw TcpIpException::Create(WSA_StartupFailed, iResult);
        }

        ~WsaInit() noexcept(false)
        {
            int iResult = WSACleanup();
            if (iResult != 0)
                throw TcpIpException::Create(WSA_CleanupFailed, iResult);
        }
    };

    WSADATA& InitializeWinsock()
    {
        return WsaInit::GetInstance().WsaData;
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
        char* buffer = new char[HEADER_SIZE + size];

        // Create header and copy it to buffer
        char* header = CreateHeader(size);
        memcpy(buffer, header, HEADER_SIZE);
        delete[] header;

        // Copy data to buffer and send it
        memcpy(buffer + HEADER_SIZE, data, size);
        int iResult = send(socket, buffer, HEADER_SIZE + static_cast<int>(size), 0);
        delete[] buffer;

        if (iResult == SOCKET_ERROR)
            throw TcpIpException::Create(SEND_DataFailed, TCP_IP_WSA_ERROR);
    }

    void Receive(const SOCKET& socket, std::stringstream& ss, const unsigned int bufferSize)
    {
        // Receive header
        char header[HEADER_SIZE];
        int iResult = recv(socket, header, HEADER_SIZE, 0);

        // iResult is number of bytes received
        if (iResult == SOCKET_ERROR)
        {
            throw TcpIpException::Create(RECEIVE_HeaderFailed, TCP_IP_WSA_ERROR);
        }

        if (iResult != HEADER_SIZE) // Received less than expected
        {
            throw TcpIpException::Create(RECEIVE_HeaderHadInvalidSize, HEADER_SIZE - iResult);
        }

        // Check header
        if (!IsHeaderValid(header))
        {
            throw TcpIpException::Create(RECEIVE_HeaderHadInvalidSignature);
        }

        // Get the the data size
        u_long dataSize = GetSizeFromHeader(header);

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
                throw TcpIpException::Create(RECEIVE_DataFailed, TCP_IP_WSA_ERROR);
            }

            if (iResult != recvSize)
            {
                delete[] buffer;
                throw TcpIpException::Create(RECEIVE_DataHadInvalidSize, recvSize - iResult);
            }

            // Write buffer to stream
            ss.write(buffer, iResult);
            dataSize -= iResult;
        }
        delete[] buffer;

        if (dataSize != 0)
        {
            throw TcpIpException::Create(RECEIVE_DataHadInvalidSize, dataSize);
        }
    }

    void CloseSocket(SOCKET& socket)
    {
        if (closesocket(socket) == SOCKET_ERROR)
            throw TcpIpException::Create(ErrorCode::SOCKET_CloseFailed, TCP_IP_WSA_ERROR);
        socket = INVALID_SOCKET;
    }

    void ReceiveHtmlRequest(const SOCKET& socket, std::stringstream& ss, const unsigned int bufferSize)
    {
        int iResult;
        char* buffer = new char[bufferSize];
        do
        {
            iResult = recv(socket, buffer, bufferSize, 0);

            if (iResult == SOCKET_ERROR)
            {
                delete[] buffer;
                throw TcpIpException::Create(RECEIVE_DataFailed, TCP_IP_WSA_ERROR);
            }

            // Write buffer to stream
            ss.write(buffer, iResult);
        } while (iResult == bufferSize);
        delete[] buffer;
    }

    void SendHtmlResponse(const SOCKET& socket, const char* data, u_long size)
    {
        send(socket, data, static_cast<int>(size), 0);
    }

    WSAEVENT CreateEventObject(const SOCKET& socket, const long networkEvents)
    {
        WSAEVENT eventObj = WSACreateEvent();
        if (eventObj == WSA_INVALID_EVENT)
            throw TcpIpException::Create(ErrorCode::EVENT_CreateFailed, TCP_IP_WSA_ERROR);

        // Set wanted flags on the event
        int iResult = WSAEventSelect(socket, eventObj, networkEvents);
        if (iResult == SOCKET_ERROR)
        {
            CloseEventObject(eventObj);
            throw TcpIpException::Create(ErrorCode::EVENT_SelectFailed, TCP_IP_WSA_ERROR);
        }

        return eventObj;
    }

    void CloseEventObject(WSAEVENT& event)
    {
        if (WSACloseEvent(event) == FALSE)
            throw TcpIpException::Create(ErrorCode::EVENT_CloseFailed, TCP_IP_WSA_ERROR);
        event = WSA_INVALID_EVENT;
    }

    IpAddress IpAddress::GetLocalAddress()
    {
        char hostName[256];
        if (gethostname(hostName, 256) != 0)
            return {0, 0, 0, 0};

        addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET; // IPv4
        hints.ai_socktype = SOCK_STREAM;

        addrinfo* result;
        int status = getaddrinfo(hostName, nullptr, &hints, &result);
        if (status != 0)
        {
            freeaddrinfo(result);
            return {0, 0, 0, 0};
        }

        sockaddr_in* addr = reinterpret_cast<sockaddr_in*>(result->ai_addr);
        IpAddress localAddress = {
            static_cast<unsigned char>(addr->sin_addr.s_addr & 0xFF),
            static_cast<unsigned char>((addr->sin_addr.s_addr >> 8) & 0xFF),
            static_cast<unsigned char>((addr->sin_addr.s_addr >> 16) & 0xFF),
            static_cast<unsigned char>((addr->sin_addr.s_addr >> 24) & 0xFF)
        };

        freeaddrinfo(result);
        return localAddress;
    }

    std::string IpAddress::ToString() const
    {
        return std::to_string(a) + "." + std::to_string(b) + "." + std::to_string(c) + "." + std::to_string(d);
    }

    IpAddress IpAddress::FromString(const std::string& str)
    {
        unsigned int a, b, c, d;
        sscanf_s(str.c_str(), "%u.%u.%u.%u", &a, &b, &c, &d);
        return {static_cast<unsigned char>(a), static_cast<unsigned char>(b), static_cast<unsigned char>(c), static_cast<unsigned char>(d)};
    }

    constexpr char Base64Alphabet[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";

    std::string IpAddress::ToPhrase() const
    {
        unsigned int num = (a << 24) + (b << 16) + (c << 8) + d;
        std::string result;
        while (num > 0)
        {
            result = Base64Alphabet[num % 64] + result;
            num /= 64;
        }
        return result;
    }

    IpAddress IpAddress::FromPhrase(const std::string& phrase)
    {
        unsigned int num = 0;
        for (char c : phrase)
        {
            num *= 64;
            for (int i = 0; i < 64; ++i)
            {
                if (Base64Alphabet[i] == c)
                {
                    num += i;
                    break;
                }
            }
        }

        return
        {
            static_cast<unsigned char>((num >> 24) & 0xFF),
            static_cast<unsigned char>((num >> 16) & 0xFF),
            static_cast<unsigned char>((num >> 8) & 0xFF),
            static_cast<unsigned char>(num & 0xFF)
        };
    }
}
