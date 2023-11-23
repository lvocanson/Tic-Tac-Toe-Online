#pragma once
#include "src/tcp-ip/TcpIpClient.h"
#include "src/core/StateMachine/StateMachine.h"

struct ISerializable;
class Thread;

enum ConnectionStateInfo
{
    Disconnected,
    Connecting,
    Failed,
    Connected
};

class ClientConnectionHandler
{
private:

    ClientConnectionHandler() = default;
    ClientConnectionHandler(const ClientConnectionHandler&) = delete;
    ~ClientConnectionHandler() = default;

public:

    static ClientConnectionHandler& GetInstance()
    {
        static ClientConnectionHandler instance;
        return instance;
    }

    void Init(Shared<StateMachine>* stateMachine);

    void Disconnect();
    void TryToConnectToServer(const std::string* adress);
    void SendDataToServer(ISerializable& data);
    void SendDataToServer(const std::string& data);

    Shared<ConnectionStateInfo>& GetConnectionInfo() { return m_IsClientConnected; }
    bool IsConnected();

    void CleanUp();

private:

    void StartThread(const std::string* adress);
    void RunClient(const std::string* adress);

private:

    Thread* m_ClientThread = nullptr;
    TcpIpClient* m_Client = nullptr;
    Shared<ConnectionStateInfo> m_IsClientConnected = ConnectionStateInfo::Disconnected;

    Shared<StateMachine>* m_StateMachine = nullptr;

    bool m_IsClientRunning = false;
    Shared<bool> m_SharedIsRunning = false;
};
