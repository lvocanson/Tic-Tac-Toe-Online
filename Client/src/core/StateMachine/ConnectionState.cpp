#include "ConnectionState.h"

ConnectionState::ConnectionState(StateMachine* stateMachine, Window* window)
	: State(stateMachine), m_Window(window)
{
	m_StateMachine = stateMachine;
}

ConnectionState::~ConnectionState()
{
	NULLPTR(m_Window);
}

void ConnectionState::OnEnter()
{
	m_ConnectButton = new ButtonComponent(sf::Vector2f(100, 100), sf::Vector2f(150, 100), sf::Color::Green, sf::Color::Red);
	m_ConnectButton->SetOnClickCallback([this]() {
		m_StateMachine->SwitchState("GameState");
	});

	m_QuitButton = new ButtonComponent(sf::Vector2f(100, 200), sf::Vector2f(150, 100), sf::Color::Green, sf::Color::Red);
	m_QuitButton->SetOnClickCallback([this]() {
		//TODO
	});

	m_IpField = new InsertFieldComponent(sf::Vector2f(100, 300), sf::Vector2f(300, 50), sf::Color::Green, sf::Color::Red, 1.0f);

	m_Window->RegisterDrawable(m_ConnectButton);
	m_Window->RegisterDrawable(m_QuitButton);
	m_Window->RegisterDrawable(m_IpField);
}

void ConnectionState::OnUpdate(float dt)
{
	m_ConnectButton->Update();
	m_QuitButton->Update();
	m_IpField->Update();
}

void ConnectionState::OnExit()
{
	m_Window->UnregisterDrawable(m_ConnectButton);
	m_Window->UnregisterDrawable(m_QuitButton);
	m_Window->UnregisterDrawable(m_IpField);

	RELEASE(m_ConnectButton);
	RELEASE(m_QuitButton);
	RELEASE(m_IpField);
}