#pragma once 
#include "State.h"
#include <map>

class StateMachine
{
public:
    StateMachine();
    ~StateMachine();

    /// <summary>
    /// Add the state ref to the dictionnary
    /// </summary>
    void AddState(const std::string& stateName, State* newState);
    const State* GetState (const std::string& stateName);
    void InitState(const std::string& initState);

    void Update(float dt);
    void Start();
    void OnReceiveData(const Json& serializeData);
    /// <summary>
    /// Switch the current state to the state with the name newState
    /// </summary>
    void SwitchState(const std::string& newState);

    inline State* GetCurrentState() const { return m_CurrentState; }

    void Cleanup();

private:

    State* m_CurrentState = nullptr;
    State* m_NextState = nullptr;

    std::map <std::string, State*> m_States;

};
