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


    GameState(StateMachine* stateMachine, Window* m_Window);
    GameState(const GameState& other) = delete;
    GameState& operator=(const GameState& other) = delete;
    ~GameState() override;

    void SetLobbyID(unsigned int id) { m_LobbyID = id; }
    void SetGameMode(const std::string& gameMode) { m_GameMode = gameMode; }
    void CheckIfMouseHoverBoard();
    bool IsMouseHoverPiece(unsigned int i);
    void SwitchPlayerTurn(const std::string& playerName);

    void UpdatePlayerTimer(float dt);
    void CheckIfTimerIsUp();

    void SendPlacedPieceToServer(unsigned int cell);

    void ClearBoard();

private:

    unsigned int m_LobbyID;
    std::string m_GameMode;

    Window* m_Window;
    ButtonComponent* m_ReturnButton = nullptr;
    GraphicBoard m_Board;

    PlayerManager m_PlayerManager;
    ScoreManager m_ScoreManager;

    float m_PlayerTurnTime = 0.0f;
    float m_MaxPlayerTurnTime = 0.0f;

    bool m_WaitingServerResponse = true;
    bool m_IsTimerOn = false;
    bool m_IsGameStarted = false;
    bool m_IsPlayerTurn = false;
    bool m_NeedToCleanBoard = false;

    GameStateUI* m_GameStateUI = nullptr;
};
