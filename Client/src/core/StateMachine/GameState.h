#pragma once
#include "StateMachine.h"
#include "State.h"

class GameState : public State
{
public:
	void OnEnter() override;
	void OnUpdate(float dt) override;
	void OnExit() override;

private:
	GameState();
	GameState(const GameState& other) = delete;
	GameState& operator=(const GameState& other) = delete;
};