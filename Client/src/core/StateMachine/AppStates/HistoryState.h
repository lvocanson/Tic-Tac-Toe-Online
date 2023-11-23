#pragma once
#include "src/core/StateMachine/StateMachine.h"
#include "src/core/Window.h"
#include "src/core/GraphicBoard.h"
#include "src/core/Components/ButtonComponent.h"

#include "src/core/Managers/GameHistoryManager.h"

class HistoryState : public State
{
public:
    void OnEnter() override;
    void OnUpdate(float dt) override;
    void OnExit() override;

    HistoryState(StateMachine* stateMachine, Window* m_Window);
    HistoryState(const HistoryState& other) = delete;
    HistoryState& operator=(const HistoryState& other) = delete;
    ~HistoryState() override;

private:

    void DisplayGame();

    void PlacePiece();
    void RemovePiece();

private:

    Window* m_Window = nullptr;

    ButtonComponent* m_NextMoveButton = nullptr;
    ButtonComponent* m_PreviousMoveButton = nullptr;
    ButtonComponent* m_NextGameButton = nullptr;
    ButtonComponent* m_PreviousGameButton = nullptr;
    ButtonComponent* m_BackToMenu = nullptr;

    GraphicBoard m_Board;

    GameData* m_CurrentGame = nullptr;

    unsigned int m_CurrentGameIndex = 0;
    unsigned int m_CurrentMoveIndex = 0;
};
