#pragma once
#include "StateMachine.h"
#include "State.h"
#include "src/core/Window.h"

class EndState : public State
{
public:
	void OnEnter() override;
	void OnUpdate(float dt) override;
	void OnExit() override;

	EndState();
	EndState(const EndState& other) = delete;
	EndState& operator=(const EndState& other) = delete;

private:
	Window* m_Window;
};