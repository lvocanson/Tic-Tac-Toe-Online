#pragma once
#include "StateMachine.h"
#include "State.h"
#include "src/core/Window.h"

class GameState : public State
{
public:
	void OnEnter() override;
	void OnUpdate(float dt) override;
	void OnExit() override;

	GameState();
	GameState(const GameState& other) = delete;
	GameState& operator=(const GameState& other) = delete;

private:
	Window* m_Window;
};