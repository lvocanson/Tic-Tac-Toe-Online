#pragma once
#include "StateMachine.h"
#include "State.h"
#include "src/core/Window.h"
#include "src/core/Components/ButtonComponent.h"
#include "src/core/Components/InsertFieldComponent.h"
#include "src/tcp-ip/TcpIpClient.h"

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

private:
	Window* m_Window;

	ButtonComponent* m_BackButton;
	InsertFieldComponent* m_IpField;

	TcpIpClient* m_Client;
};