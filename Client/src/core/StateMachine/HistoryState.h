#pragma once
#include "StateMachine.h"
#include "State.h"

class HistoryState : public State
{
public:
	void OnEnter() override;
	void OnUpdate(float dt) override;
	void OnExit() override;

private:
	HistoryState();
	HistoryState(const HistoryState& other) = delete;
	HistoryState& operator=(const HistoryState& other) = delete;
};