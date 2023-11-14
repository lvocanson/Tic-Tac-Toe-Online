#include "StateMachine.h"

StateMachine::StateMachine()
{
	m_CurrentState = nullptr;
}

void StateMachine::InitState(State* initState)
{
	m_CurrentState = initState;
}

void StateMachine::Start()
{
	if (!m_CurrentState) 
	{
		std::cout << "State machine isn't init" << std::endl;
		return;
	}

	m_CurrentState->OnEnter();
}

void StateMachine::AddState(std::string stateName, State* newState)
{
	m_States.insert(std::pair<std::string, State*>(stateName, newState));
}

void StateMachine::SwitchState(std::string newState)
{
	State* state = m_States[newState];
	if (!state)
	{
		std::cout << "State not found" << std::endl;
		return;
	}

	m_CurrentState->OnExit();
	m_CurrentState = state;
	m_CurrentState->OnEnter();
}