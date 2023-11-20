#pragma once
#include "src/core/StateMachine/StateMachine.h"
#include "Managers/InputHandler.h"
#include "src/tcp-ip/TcpIpClient.h"

class Window;
class sf::Shape;
class TcpIpClient;

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
    void Shutdown() { m_IsRunning = false; }

    void Send(const std::string& data);

    void Connection(const std::string& ip);
    void SetPlayerName(const std::string& name) { m_PlayerName = name; }

private: // Methods
    /// Update the ClientApp. Called once per frame.
    void Update(sf::Time delta);

    /// Perform any cleanup tasks (e.g. delete pointers). Called before Run() returns.
    void Cleanup();

private: // Fields
    bool m_IsRunning = false;

    Window* m_Window = nullptr;
    StateMachine* m_StateMachine;

    InputHandler m_InputHandler;
    std::string m_PlayerName;
    TcpIpClient* m_Client;
};