#pragma once
#include "src/core/StateMachine/StateMachine.h"
#include "src/core/Window.h"
#include "src/core/Components/ButtonComponent.h"

class SelectState : public State
{
public:
    void OnEnter() override;
    void OnUpdate(float dt) override;
    void OnExit() override;

    SelectState(StateMachine* stateMachine, Window* window);
    SelectState(const SelectState& other) = delete;
    SelectState& operator=(const SelectState& other) = delete;
    ~SelectState();

private:
    Window* m_Window;

    ButtonComponent* m_CircleShape;
    ButtonComponent* m_SquareShape;
    ButtonComponent* m_Viewer;
};