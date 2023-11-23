#pragma once
#include "src/core/StateMachine/StateMachine.h"
#include "src/core/Window.h"
#include "src/core/Components/ButtonComponent.h"
#include "src/core/Components/InsertFieldComponent.h"

class ConnectionState : public State
{
public:
    void OnEnter() override;
    void OnUpdate(float dt) override;
    void OnExit() override;


    ConnectionState(StateMachine* stateMachine, Window* window);
    ConnectionState(const ConnectionState& other) = delete;
    ConnectionState& operator=(const ConnectionState& other) = delete;
    ~ConnectionState();

    bool IsValidIpAddress(const char* ip);

private:

    bool m_IsTryingToConnect = false;

    Window* m_Window = nullptr;

    std::string m_Name = "";

    ButtonComponent* m_BackButton = nullptr;
    ButtonComponent* m_ConnectButton = nullptr;

    InsertFieldComponent* m_IpField = nullptr;
    InsertFieldComponent* m_NameField = nullptr;
};
