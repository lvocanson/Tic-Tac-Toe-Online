#include "MenuState.h"

MenuState::MenuState(StateMachine* stateMachine, Window* window)
	: State(stateMachine)
	, m_Window(window)
	, m_PlayButton()
	, m_HistoryButton()
	, m_QuitButton()
{
	m_StateMachine = stateMachine;
}

void MenuState::OnEnter()
{
	m_PlayButton = new ButtonComponent(100, 100, 200, 100, sf::Color::Blue, sf::Color::Red);
	m_PlayButton->SetOnClickCallback([this]() {
		m_StateMachine->SwitchState("GameState");
	});

	m_HistoryButton = new ButtonComponent(100, 300, 200, 100, sf::Color::Green, sf::Color::Red);
	m_HistoryButton->SetOnClickCallback([this]() {
		m_StateMachine->SwitchState("HistoryState");
	});

	m_QuitButton = new ButtonComponent(100, 500, 200, 100, sf::Color::Red, sf::Color::Red);
	m_QuitButton->SetOnClickCallback([this]() {
		// TODO: Quit the game
	});

	m_Window->RegisterDrawable(m_PlayButton);
	m_Window->RegisterDrawable(m_HistoryButton);
	m_Window->RegisterDrawable(m_QuitButton);
}

void MenuState::OnUpdate(float dt)
{
	if (m_PlayButton->IsMouseOver(m_Window) && (sf::Mouse::isButtonPressed(sf::Mouse::Left)))
	{
		m_StateMachine->SwitchState("GameState");
	}

	if (m_HistoryButton->IsMouseOver(m_Window) && (sf::Mouse::isButtonPressed(sf::Mouse::Left)))
	{
		m_StateMachine->SwitchState("HistoryState");
	}

	if (m_QuitButton->IsMouseOver(m_Window) && (sf::Mouse::isButtonPressed(sf::Mouse::Left)))
	{
		// TODO
	}
}

void MenuState::OnExit()
{
	m_Window->UnregisterDrawable(m_PlayButton);
	m_Window->UnregisterDrawable(m_HistoryButton);
	m_Window->UnregisterDrawable(m_QuitButton);
}