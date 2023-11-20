#include "SelectState.h"

SelectState::SelectState(StateMachine* stateMachine, Window* window) 
    : State(stateMachine)
    , m_Window(window)
    , m_CircleShape()
    , m_SquareShape()
    , m_Viewer()
{
    m_StateMachine = stateMachine;
}

SelectState::~SelectState()
{
    NULLPTR(m_Window);
}

void SelectState::OnEnter()
{
    m_CircleShape = new ButtonComponent(sf::Vector2f(100, 100), sf::Vector2f(200, 100), sf::Color::Cyan, sf::Color::Red);
    m_CircleShape->SetButtonText("Circle", sf::Color::White, 30, TextAlignment::Center);
    m_CircleShape->SetOnClickCallback([this]() {
            m_StateMachine->SwitchState("GameState");
        });
    
    m_SquareShape = new ButtonComponent(sf::Vector2f(100, 300), sf::Vector2f(200, 100), sf::Color::Yellow, sf::Color::Red);
    m_SquareShape->SetButtonText("Square", sf::Color::White, 30, TextAlignment::Center);
    m_SquareShape->SetOnClickCallback([this](){
            m_StateMachine->SwitchState("GameState");
        });

    m_Viewer = new ButtonComponent(sf::Vector2f(100, 500), sf::Vector2f(200, 100), sf::Color::Green, sf::Color::Red);
    m_Viewer->SetButtonText("Viewer", sf::Color::White, 30, TextAlignment::Center);
    m_Viewer->SetOnClickCallback([this](){
            m_StateMachine->SwitchState("GameState");
        });

    m_Window->RegisterDrawable(m_CircleShape);
    m_Window->RegisterDrawable(m_SquareShape);
    m_Window->RegisterDrawable(m_Viewer);
}

void SelectState::OnUpdate(float dt)
{
    m_CircleShape->Update();
    m_SquareShape->Update();
}

void SelectState::OnExit()
{
    m_Window->UnregisterDrawable(m_CircleShape);
    m_Window->UnregisterDrawable(m_SquareShape);
    m_Window->UnregisterDrawable(m_Viewer);

    RELEASE(m_CircleShape);
    RELEASE(m_SquareShape);
    RELEASE(m_Viewer);
}

