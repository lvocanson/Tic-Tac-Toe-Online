#pragma once 

class StateMachine;

class State
{
public:
	
	State(StateMachine* stateMachine) : m_StateMachine(stateMachine) { };
	virtual ~State() { m_StateMachine = nullptr; };

	virtual void OnEnter() = 0;
	virtual void OnUpdate(float dt) = 0;
	virtual void OnExit() = 0;
	virtual void OnReceiveData(const Json& serializeData) {};

protected:
	StateMachine* m_StateMachine;

};
