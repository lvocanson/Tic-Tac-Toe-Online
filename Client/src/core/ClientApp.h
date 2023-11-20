#pragma once
#include "src/core/StateMachine/StateMachine.h"
#include "Managers/InputHandler.h"
#include "src/tcp-ip/TcpIpClient.h"

class Window;
class sf::Shape;
class TcpIpClient;


typedef struct PullServerThreadData
{
    TcpIpClient* Client;
    StateMachine* StateMachine;
    Shared<bool>* IsRunning;
} PULLSERVERDATA, * PPULLSERVERDATA;

class ClientApp
{
    // Private constructor for singleton
    ClientApp() = default;
    ClientApp(const ClientApp&) = delete;
    ~ClientApp() = default;

public:
    /// <summary>
    /// Get the singleton instance of the ClientApp. The instance is created on the first call to this method.
    /// </summary>
    static ClientApp& GetInstance()
    {
        static ClientApp instance;
        return instance;
    }

    /// <summary>
    /// Initialize the ClientApp. This method must be called before calling Run().
    /// </summary>
    void Init();
    /// <summary>
    /// Run the ClientApp. This method will not return until the ClientApp is shut down.
    /// </summary>
    void Run();
    /// <summary>
    /// Shutdown the ClientApp. This method will cause Run() to return.
    /// </summary>
    void Shutdown() { m_IsRunning.WaitGet().operator->() = false; }

    void Send(const std::string& data);

private: // Methods
    /// Update the ClientApp. Called once per frame.
    void Update(sf::Time delta);

    /// Perform any cleanup tasks (e.g. delete pointers). Called before Run() returns.
    void Cleanup();

private: // Fields

    Shared<bool> m_IsRunning;

    Window* m_Window = nullptr;
    StateMachine* m_StateMachine = nullptr;
    HANDLE m_PullServerThread = nullptr;
    PPULLSERVERDATA m_PullServerData = nullptr;
    InputHandler m_InputHandler;

    TcpIpClient* m_Client = nullptr;
};
