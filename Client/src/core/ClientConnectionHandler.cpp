#include "src/pch.h"
#include "ClientConnectionHandler.h"
#include "threading/Thread.h"

void ClientConnectionHandler::Init(Shared<StateMachine>* stateMachine)
{
    m_StateMachine = stateMachine;
}

void ClientConnectionHandler::Disconnect()
{
    m_IsClientConnected.WaitGet().Get() = Disconnected;

    if (m_ClientThread != nullptr)
    {
        m_SharedIsRunning.WaitGet().Get() = false;
        m_ClientThread->Wait();
        RELEASE(m_ClientThread);
    }
}

void ClientConnectionHandler::TryToConnectToServer(const std::string* adress)
{
    Disconnect();

    DebugLog("Try to connect to " + *adress + "...\n");

    m_IsClientConnected.WaitGet().Get() = Connecting;

    StartThread(adress);
}

void ClientConnectionHandler::StartThread(const std::string* ipAdress)
{
    m_SharedIsRunning.WaitGet().Get() = true;

    m_ClientThread = Thread::Create([](LPVOID ip) -> DWORD
    {
        ClientConnectionHandler::GetInstance().RunClient(static_cast<std::string*>(ip));

        return 0;
    }, ipAdress, true);
}

void ClientConnectionHandler::RunClient(const std::string* adress)
{
    try
    {
        try
        {
            m_Client = new TcpIpClient();
            m_Client->Connect(adress->c_str(), DEFAULT_PORT);
            m_IsClientRunning = true;
            m_IsClientConnected.WaitGet().Get() = Connected;
            DebugLog("Connected to server!\n");
        }
        catch (const TcpIp::TcpIpException& e)
        {
            DebugLog("Failed to connect to server: " + std::string(e.what()) + "\n");
            m_IsClientConnected.WaitGet().Get() = Failed;
            m_IsClientRunning = false;
        }

        std::stringstream ss;
        while (m_IsClientRunning)
        {
            try
            {
                while (m_Client->FetchPendingData(ss))
                {
                    Json j;
                    try
                    {
                        j = Json::parse(ss.str());
                    }
                    catch (const std::exception& e)
                    {
                        DebugLog("Failed to parse JSON from server!");
                        return;
                    }
                    if (!j.contains("Type"))
                    {
                        DebugLog("Json does not contain a Type!\n");
                        return;
                    }
                    m_StateMachine->WaitGet()->OnReceiveData(j);
                    ss.str(std::string());
                }
            }
            catch (const TcpIp::TcpIpException& e)
            {
                DebugLog("Failed to fetch data from server: " + std::string(e.what()) + "\n");
                m_IsClientRunning = false;
            }

            if (!m_Client->IsConnected())
            {
                DebugLog("Disconnected from server!\n");
                m_IsClientRunning = false;
                m_IsClientConnected.WaitGet().Get() = Disconnected;
            }

            auto sharedRunning = m_SharedIsRunning.TryGet();
            if (sharedRunning.IsValid())
                m_IsClientRunning &= sharedRunning.Get();
        }

        m_SharedIsRunning.WaitGet().Get() = false;
        m_Client->Disconnect();
        RELEASE(m_Client);
    }
    catch (...)
    {
        DebugLog("Global catch failed \n");
        DebugLog("The thread is shutting down \n");

        m_SharedIsRunning.WaitGet().Get() = false;
        m_IsClientConnected.WaitGet().Get() = Disconnected;

        if (m_Client)
            m_Client->Disconnect();

        RELEASE(m_Client);
    }
}


void ClientConnectionHandler::SendDataToServer(const std::string& data)
{
    if (data.empty())
    {
        DebugLog("Data is empty!");
        return;
    }

    if (!m_Client || !m_IsClientConnected.WaitGet().Get() == Connected)
    {
        DebugLog("Client isn't connected to server !");
        return;
    }

    m_Client->Send(data);
}

bool ClientConnectionHandler::IsConnected()
{
    if (m_Client && m_IsClientConnected.WaitGet().Get() == Connected)
        return true;

    return false;
}

void ClientConnectionHandler::CleanUp()
{
    if (m_ClientThread)
    {
        m_ClientThread->Start();
        m_SharedIsRunning.WaitGet().Get() = false;
        m_ClientThread->Wait();
        RELEASE(m_ClientThread);
    }
}