#pragma once
#include "StateMachine.h"
#include "State.h"
#include "src/core/Window.h"
#include "src/core/Components/ButtonComponent.h"

class HistoryState : public State
{
public:
	void OnEnter() override;
	void OnUpdate(float dt) override;
	void OnExit() override;

	HistoryState(StateMachine* stateMachine, Window* m_Window);
	HistoryState(const HistoryState& other) = delete;
	HistoryState& operator=(const HistoryState& other) = delete;
	~HistoryState();

private:
	Window* m_Window;

	ButtonComponent* m_RArrowButton;
	ButtonComponent* m_LArrowButton;
	ButtonComponent* m_BackToMenu;
};