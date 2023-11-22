#pragma once
#include "src/core/StateMachine/StateMachine.h"
#include "src/core/Window.h"
#include "src/core/Components/ButtonComponent.h"

class EndState : public State
{
public:
    void OnEnter() override;
    void OnUpdate(float dt) override;
    void OnExit() override;

    EndState(StateMachine* stateMachine, Window* m_Window);
    EndState(const EndState& other) = delete;
    EndState& operator=(const EndState& other) = delete;
    ~EndState();

    void Cleanup();

private:
    Window* m_Window;    
    ButtonComponent* m_ButtonComponent;

};