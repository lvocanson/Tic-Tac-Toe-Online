#pragma once
#include "game/GameMode.h"
#include "src/core/StateMachine/StateMachine.h"
#include "Managers/InputHandler.h"
#include "Managers/TimeManager.h"
#include "Player.h"
#include "src/core/Managers/GameHistoryManager.h"

class Window;
class sf::Shape;

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

    static GameSettings& GetGameSettings() { return GetInstance().m_GameSettings; }

    void SetPlayerName(const std::string& name) { m_PlayerName = name; }

    Player* GetCurrentPlayer() { return m_Player; }

    static GameHistoryManager* GetHistoryManager() { return GetInstance().m_GameHistoryManager; }

private: // Methods

    /// Update the ClientApp. Called once per frame.
    void Update(float delta);

    /// Perform any cleanup tasks (e.g. delete pointers). Called before Run() returns.
    void Cleanup();

private: // Fields

    bool m_IsRunning = false;
    
    TimeManager m_TimeManager;
    Window* m_Window = nullptr;
    Shared<StateMachine>* m_StateMachine = nullptr;
    GameSettings m_GameSettings;
    InputHandler m_InputHandler;
    Player* m_Player;
    GameHistoryManager* m_GameHistoryManager;

    std::string m_PlayerName;
};
