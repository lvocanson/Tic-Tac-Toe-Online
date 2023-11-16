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

MenuState::~MenuState()
{
	NULLPTR(m_Window);
}

void MenuState::OnEnter()
{
	m_PlayButton = new ButtonComponent(sf::Vector2f(100, 100), sf::Vector2f(200, 100), sf::Color::Blue, sf::Color::Red, "Play", sf::Color::White, 30, TextAlignment::Center);
	m_PlayButton->SetOnClickCallback([this]() {
		m_StateMachine->SwitchState("GameState");
	});

	m_HistoryButton = new ButtonComponent(sf::Vector2f(100, 300), sf::Vector2f(200, 100), sf::Color::Green, sf::Color::Red, "History", sf::Color::White, 30, TextAlignment::Center);
	m_HistoryButton->SetOnClickCallback([this]() {
		m_StateMachine->SwitchState("HistoryState");
	});

	m_QuitButton = new ButtonComponent(sf::Vector2f(100, 500), sf::Vector2f(200, 100), sf::Color::Red, sf::Color::Red, "Quit", sf::Color::White, 30, TextAlignment::Center);
	m_QuitButton->SetOnClickCallback([this]() {
		// TODO: Quit the game
	});

	m_Window->RegisterDrawable(m_PlayButton);
	m_Window->RegisterDrawable(m_HistoryButton);
	m_Window->RegisterDrawable(m_QuitButton);
}

void MenuState::OnUpdate(float dt)
{
	m_PlayButton->Update(m_Window);
	m_HistoryButton->Update(m_Window);
	m_QuitButton->Update(m_Window);
}

void MenuState::OnExit()
{
	m_Window->UnregisterDrawable(m_PlayButton);
	m_Window->UnregisterDrawable(m_HistoryButton);
	m_Window->UnregisterDrawable(m_QuitButton);

	RELEASE(m_PlayButton);
	RELEASE(m_HistoryButton);
	RELEASE(m_QuitButton);
}
