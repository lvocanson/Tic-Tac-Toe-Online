#pragma once
#include "src/core/StateMachine/StateMachine.h"
#include "src/core/Components/ButtonComponent.h"
#include "src/core/GraphicBoard.h"
#include "src/core/UIState/GameStateUI.h"

#include "src/core/Managers/PlayerManager.h"
#include "src/core/Managers/ScoreManager.h"


class sf::Shape;

class GameState : public State
{
public:
    void OnEnter() override;
    void OnUpdate(float dt) override;
    void OnExit() override;
    void OnReceiveData(const Json& serializeData) override;

    void IsServerLobbyFull();

    void StartGame();

    GameState(StateMachine* stateMachine, Window* m_Window);
    GameState(const GameState& other) = delete;
    GameState& operator=(const GameState& other) = delete;
    ~GameState() override;

    void SetLobbyID(unsigned int id) { m_LobbyID = id; }
    void CheckIfMouseHoverBoard();
    bool IsMouseHoverPiece(unsigned int i);
    void PlacePlayerPieceOnBoard(unsigned int cell);
    void SendGameFinishedToServer(const std::string& string);
    void WinCheck();
    void SwitchPlayerTurn();

    void UpdatePlayerTimer(float dt);
    void CheckIfTimerIsUp();

    void SendPlacedPieceToServer(unsigned int cell);

    void ClearBoard();

private:

    unsigned int m_LobbyID;

    Window* m_Window;
    ButtonComponent* m_ReturnButton = nullptr;
    GraphicBoard m_Board;

    PlayerManager m_PlayerManager;
    ScoreManager m_ScoreManager;

    float m_PlayerTurnTime = 0.0f;
    float m_MaxPlayerTurnTime = 0.0f;

    bool m_IsTimerOn = false;
    bool m_IsGameStarted = false;
    bool m_IsPlayerTurn = false;

    GameStateUI* m_GameStateUI;
};
