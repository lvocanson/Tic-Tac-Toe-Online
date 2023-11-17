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
	NULLPTR(m_Window);
}

void HistoryState::OnEnter()
{
    m_RArrowButton = new ButtonComponent(sf::Vector2f(50, 50), sf::Vector2f(50, 50), sf::Color::Green, sf::Color::Red);
    m_RArrowButton->SetButtonText(">", sf::Color::White, 30, TextAlignment::Center);
    m_RArrowButton->SetOnClickCallback([this]() {
        // TODO
        });

    m_LArrowButton = new ButtonComponent(sf::Vector2f(150, 50), sf::Vector2f(50, 50), sf::Color::Green, sf::Color::Red);
    m_LArrowButton->SetButtonText("<", sf::Color::White, 30, TextAlignment::Center);
    m_LArrowButton->SetOnClickCallback([this]() {
        // TODO
        });

    m_BackToMenu = new ButtonComponent(sf::Vector2f(150, 400), sf::Vector2f(200, 100), sf::Color::Blue, sf::Color::Red);
    m_BackToMenu->SetButtonText("Back to Menu", sf::Color::White, 30, TextAlignment::Center);
    m_BackToMenu->SetOnClickCallback([this]() {
        m_StateMachine->SwitchState("MenuState");
        });

    m_Window->RegisterDrawable(m_RArrowButton);
    m_Window->RegisterDrawable(m_LArrowButton);
    m_Window->RegisterDrawable(m_BackToMenu);
}


void HistoryState::OnUpdate(float dt)
{
	m_RArrowButton->Update();
	m_LArrowButton->Update();
	m_BackToMenu->Update();
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
