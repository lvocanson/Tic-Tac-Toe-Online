#pragma once
#include "StateMachine.h"
#include "State.h"

class EndState : public State
{
public:
	void OnEnter() override;
	void OnUpdate(float dt) override;
	void OnExit() override;

private:
	EndState();
	EndState(const EndState& other) = delete;
	EndState& operator=(const EndState& other) = delete;
};