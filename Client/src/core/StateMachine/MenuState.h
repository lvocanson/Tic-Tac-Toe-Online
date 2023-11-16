#pragma once
#include "StateMachine.h"
#include "State.h"
#include "src/core/Window.h"
#include "src/core/Components/ButtonComponent.h"


class MenuState : public State
{
public:
	void OnEnter() override;
	void OnUpdate(float dt) override;
	void OnExit() override;

	MenuState(StateMachine* stateMachine, Window* window);
	MenuState(const MenuState& other) = delete;
	MenuState& operator=(const MenuState& other) = delete;
	~MenuState();

	void Cleanup();

private:
	Window* m_Window;

	ButtonComponent* m_PlayButton;
	ButtonComponent* m_HistoryButton;
	ButtonComponent* m_QuitButton;
};