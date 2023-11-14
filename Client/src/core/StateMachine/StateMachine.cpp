#include "StateMachine.h"

StateMachine::StateMachine()
{
	m_CurrentState = nullptr;
}

void StateMachine::InitState(State* initState)
{
	m_CurrentState = initState;
	m_CurrentState->OnEnter();
}

void StateMachine::SetState(State& newState)
{
	m_CurrentState->OnExit();
	m_CurrentState = &newState;
	m_CurrentState->OnEnter();
}