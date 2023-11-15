#pragma once
#include "StateMachine.h"
#include "State.h"
#include "src/core/Window.h"

class HistoryState : public State
{
public:
	void OnEnter() override;
	void OnUpdate(float dt) override;
	void OnExit() override;

	HistoryState();
	HistoryState(const HistoryState& other) = delete;
	HistoryState& operator=(const HistoryState& other) = delete;

private:
	Window* m_Window;
};