#pragma once 
#include "State.h"

class StateMachine
{
public:
	StateMachine();

	void InitState(State* initState);

	inline State* GetCurrentState() const { return m_CurrentState; }
	void SetState(State& newState);

private:

	State* m_CurrentState;

};
