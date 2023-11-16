#pragma once
#include "GraphicBoard.h"
#include <SFML/System/Time.hpp>

#include "Managers/GameStateUI.h"
#include "Managers/ScoreManager.h"
#include "Managers/PlayerManager.h"
#include <GameMode.h>

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
    static ClientApp& GetInstance() { static ClientApp instance; return instance; }

    void DrawBoard();
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

    void CheckIfMouseHoverBoard();
    bool IsMouseHoverPiece(unsigned int i);
    void PlacePlayerPieceOnBoard(unsigned int cell);
    void SetGraphicalPiece(unsigned cell, const Player* currentPlayer);
    void SwitchPlayerTurn();

    void ClearBoard();
  

    /// Perform any cleanup tasks (e.g. delete pointers). Called before Run() returns.
    void Cleanup();

private: // Fields
    bool m_IsRunning = false;

    Window* m_Window = nullptr;
    GameStateUI* m_GameStateUI = nullptr;
    GameSettings m_GameSettings;
    InputHandler m_InputHandler;
    GraphicBoard m_Board;
    PlayerManager m_PlayerManager;
    ScoreManager m_ScoreManager;

    sf::Time m_PlayerTurnTimer = sf::seconds(0);

    std::vector<sf::Drawable*> m_GamePieces;

};
