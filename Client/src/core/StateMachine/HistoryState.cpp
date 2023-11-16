#include "HistoryState.h"

HistoryState::HistoryState(StateMachine* stateMachine, Window* m_Window)
	: State(stateMachine)
	, m_Window(m_Window)
	, m_RArrowButton()
	, m_LArrowButton()
	, m_BackToMenu()
{
	m_StateMachine = stateMachine;
}

HistoryState::~HistoryState()
{
	Cleanup();
}

void HistoryState::OnEnter()
{
	m_RArrowButton = new ButtonComponent(50, 50, 50, 50, sf::Color::Green, sf::Color::Red, ">", sf::Color::White, 30, TextAlignment::Center);
	m_RArrowButton->SetOnClickCallback([this]() {
		// TODO
	});

	m_LArrowButton = new ButtonComponent(150, 50, 50, 50, sf::Color::Green, sf::Color::Red, "<", sf::Color::White, 30, TextAlignment::Center);
	m_LArrowButton->SetOnClickCallback([this]() {
		// TODO
	});

	m_BackToMenu = new ButtonComponent(150, 400, 200, 100, sf::Color::Blue, sf::Color::Red, "Back to Menu", sf::Color::White, 30, TextAlignment::Center);
	m_BackToMenu->SetOnClickCallback([this]() {
		m_StateMachine->SwitchState("MenuState");
	});

	m_Window->RegisterDrawable(m_RArrowButton);
	m_Window->RegisterDrawable(m_LArrowButton);
	m_Window->RegisterDrawable(m_BackToMenu);
}

void HistoryState::OnUpdate(float dt)
{
	if (m_BackToMenu->IsMouseOver(m_Window) && (sf::Mouse::isButtonPressed(sf::Mouse::Left)))
	{
		m_StateMachine->SwitchState("MenuState");
		return;
	}
}

void HistoryState::OnExit()
{
	m_Window->UnregisterDrawable(m_RArrowButton);
	m_Window->UnregisterDrawable(m_LArrowButton);
	m_Window->UnregisterDrawable(m_BackToMenu);

	RELEASE(m_RArrowButton);
	RELEASE(m_LArrowButton);
	RELEASE(m_BackToMenu);
}

void HistoryState::Cleanup()
{
	NULLPTR(m_Window);
}
