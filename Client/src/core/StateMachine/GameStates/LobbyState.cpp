#include "LobbyState.h"
#include "../Client/src/core/ClientApp.h"
#include "../TicTacToe/IDGenerator.h"

LobbyState::LobbyState(StateMachine* stateMachine, Window* window) 
    : State(stateMachine)
    , m_Window(window)
    , m_ReturnButton(nullptr)
    , m_LobbyButton(nullptr)
    , m_GameStateUI(nullptr)
{
    m_StateMachine = stateMachine;
}

LobbyState::~LobbyState()
{
    NULLPTR(m_Window);
}


void LobbyState::OnEnter()
{
    m_LobbyButton = new ButtonComponent(sf::Vector2f(100, 100), sf::Vector2f(200, 100), sf::Color::Blue, sf::Color::White);
    m_LobbyButton->SetButtonText("Lobby 1", sf::Color::White, 30, TextAlignment::Center);
    m_LobbyButton->SetOnClickCallback([&]()
            {
                JoinLobby();
            });
    m_Window->RegisterDrawable(m_LobbyButton);
    

    m_ReturnButton = new ButtonComponent(sf::Vector2f(100, 500), sf::Vector2f(200, 100), sf::Color::Red, sf::Color::White);
    m_ReturnButton->SetButtonText("Return", sf::Color::White, 30, TextAlignment::Center);
    m_ReturnButton->SetOnClickCallback([this]()
        {
            LeaveLobby();
            m_StateMachine->SwitchState("MenuState");
        });

    m_Window->RegisterDrawable(m_ReturnButton);
}

void LobbyState::OnUpdate(float dt)
{
    m_LobbyButton->Update();
    m_ReturnButton->Update(); 
}

void LobbyState::OnExit()
{
    m_Window->UnregisterDrawable(m_LobbyButton);
    RELEASE(m_LobbyButton);

    m_Window->UnregisterDrawable(m_ReturnButton);
    RELEASE(m_ReturnButton);
}


void LobbyState::JoinLobby()
{
    Json j;
    j["Join lobby"] = true;
    ClientApp::GetInstance().Send(j.dump());
}

void LobbyState::LeaveLobby()
{

}