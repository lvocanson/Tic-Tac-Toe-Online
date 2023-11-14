#pragma once
#include "StateMachine.h"
#include "State.h"

class MenuState : public State
{
public:
	void OnEnter() override;
	void OnUpdate(float dt) override;
	void OnExit() override;

private:
	MenuState();
	MenuState(const MenuState& other) = delete;
	MenuState& operator=(const MenuState& other) = delete;
};