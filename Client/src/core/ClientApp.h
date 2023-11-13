#pragma once
#include "TicTacToe.h"

class Window;

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
    static ClientApp& GetInstance() { static ClientApp instance; return instance; }

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

private: // Methods
    /// Update the ClientApp. Called once per frame.
    void Update(sf::Time delta);
    void SwitchPlayerTurn();
    void PlacePlayerPieceOnBoard(size_t i);

    void CheckIfMouseHoverBoard();
    bool IsMouseHoverPiece(size_t i) const;

    /// Perform any cleanup tasks (e.g. delete pointers). Called before Run() returns.
    void Cleanup();

private: // Fields
    bool m_IsRunning = false;
    bool m_IsGameFinished = false;
    Window* m_Window = nullptr;

    TicTacToe::Board* m_Board = nullptr;
    TicTacToe::Player* m_PlayerOne = nullptr;
    TicTacToe::Player* m_PlayerTwo = nullptr;

    sf::Time m_PlayerTurnDelay = sf::seconds(0);

    bool m_IsPlayerOneTurn = true;

};