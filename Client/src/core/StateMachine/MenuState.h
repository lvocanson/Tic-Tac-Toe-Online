#pragma once
#include "StateMachine.h"
#include "State.h"
#include "src/core/Window.h"


class MenuState : public State
{
public:
	void OnEnter() override;
	void OnUpdate(float dt) override;
	void OnExit() override;

	MenuState();
	MenuState(const MenuState& other) = delete;
	MenuState& operator=(const MenuState& other) = delete;

private:
	Window* m_Window;
};