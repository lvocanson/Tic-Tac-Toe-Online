#include "StateMachine.h"

StateMachine::StateMachine()
    : m_States()
{
}

StateMachine::~StateMachine()
{
    Cleanup();
}

void StateMachine::InitState(const std::string& initState)
{
    m_CurrentState = m_States[initState];
}

void StateMachine::AddState(const std::string& stateName, State* newState)
{
    m_States.insert(std::pair(stateName, newState));
}

const State* StateMachine::GetState(const std::string& stateName)
{
    const State* state = m_States[stateName];
    if (!state)
    {
        std::cout << "State not found" << std::endl;
        return nullptr;
    }

    return state;
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

void StateMachine::OnReceiveData(const Json& data)
{
    m_CurrentState->OnReceiveData(data);
}

void StateMachine::Update(float dt)
{
    if (m_NextState)
    {
        m_CurrentState->OnExit();
        m_CurrentState = m_NextState;
        m_CurrentState->OnEnter();
        NULLPTR(m_NextState);
    }
    m_CurrentState->OnUpdate(dt);
}

void StateMachine::SwitchState(const std::string& newState)
{
    State* state = m_States[newState];
    if (!state)
    {
        std::cout << "State not found" << std::endl;
        return;
    }

    m_NextState = state;
}

void StateMachine::Cleanup()
{
    if (m_CurrentState)
    {
        m_CurrentState->OnExit();
    }

    for (auto state : m_States)
    {
        RELEASE(state.second);
    }

    m_States.clear();

}
