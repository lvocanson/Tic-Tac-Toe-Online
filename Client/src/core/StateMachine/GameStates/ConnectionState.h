#pragma once
#include "src/core/StateMachine/StateMachine.h"
#include "src/core/Window.h"
#include "src/core/Components/ButtonComponent.h"
#include "src/core/Components/InsertFieldComponent.h"
#include "src/tcp-ip/TcpIpClient.h"

class TcpIpClient;

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

	void Send(const std::string& data);

private:
	Window* m_Window;

	ButtonComponent* m_BackButton;
	InsertFieldComponent* m_IpField;

	TcpIpClient* m_Client;
};