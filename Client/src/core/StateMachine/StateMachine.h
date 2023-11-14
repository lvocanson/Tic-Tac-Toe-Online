#pragma once 
#include "State.h"
#include <map>

class StateMachine
{
public:
	StateMachine();
	 
	/// <summary>
	/// Add the state ref to the dictionnary
	/// </summary>
	void AddState(std::string stateName, State* newState);

	void InitState(State* initState);
	void Start();

	/// <summary>
	/// Switch the current state to the state with the name newState
	/// </summary>
	void SwitchState(std::string newState);

	inline State* GetCurrentState() const { return m_CurrentState; }

private:

	State* m_CurrentState;

	std::map <std::string, State*> m_States;

};
