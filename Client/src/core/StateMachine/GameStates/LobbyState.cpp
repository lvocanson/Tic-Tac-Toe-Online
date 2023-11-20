#include "LobbyState.h"
#include "../Client/src/core/ClientApp.h"
#include "../TicTacToe/IDGenerator.h"

LobbyState::LobbyState(StateMachine* stateMachine, Window* window) 
    : State(stateMachine)
    , m_Window(window)
    , m_MaxLobbyNumber(3)
    , m_CurrentLobby(nullptr)
    , m_IsLobbyCreated(false)
{
    m_StateMachine = stateMachine;
}

LobbyState::~LobbyState()
{
    for (auto& lb : m_Lobbies)
    {
        RELEASE(lb);
    };
    NULLPTR(m_Window);
}

void LobbyState::OnEnter()
{
    for (size_t i = 0; i < m_MaxLobbyNumber; i++)
    {
        ButtonComponent* lbButton = new ButtonComponent(sf::Vector2f(100, 100 + (i * 100)), sf::Vector2f(200, 100), sf::Color::Blue, sf::Color::White);
        lbButton->SetButtonText("Lobby " + std::to_string(i), sf::Color::White, 30, TextAlignment::Center);
        lbButton->SetOnClickCallback([&](){

            });
        m_LobbyButtons.push_back(lbButton);
        m_Window->RegisterDrawable(lbButton);
    }

    if (!m_IsLobbyCreated)
        CreateLobbies();

    m_ReturnButton = new ButtonComponent(sf::Vector2f(100, 500), sf::Vector2f(200, 100), sf::Color::Red, sf::Color::White);
    m_ReturnButton->SetButtonText("Return", sf::Color::White, 30, TextAlignment::Center);
    m_ReturnButton->SetOnClickCallback([this]()
        {
            m_StateMachine->SwitchState("MenuState");
        });
    m_Window->RegisterDrawable(m_ReturnButton);
}

void LobbyState::OnUpdate(float dt)
{
    m_ReturnButton->Update();
    
    for (auto& lbButton : m_LobbyButtons)
    {
        if (lbButton == nullptr)
        {
            return;
        }
        lbButton->Update();
    }
}

void LobbyState::OnExit()
{

    for (auto& lbButton : m_LobbyButtons)
    {
        m_Window->UnregisterDrawable(lbButton);
        RELEASE(lbButton);
    }

    m_Window->UnregisterDrawable(m_ReturnButton);
    RELEASE(m_ReturnButton);
}


void LobbyState::CreateLobbies()
{
    Json j;
    Lobby* newLobby = new Lobby();
    newLobby->ID = IDGenerator::GenerateLobbyID();
    newLobby->PlayerCount = 0;
    m_Lobbies.push_back(newLobby);
    j["Lobbies"]["ID"] = newLobby->ID;
    j["Lobbies"]["PlayerCount"] = newLobby->PlayerCount;
    m_IsLobbyCreated = true;
    j["IsLobbyCreated"] = m_IsLobbyCreated;
    ClientApp::GetInstance().Send("Create " + std::to_string(m_MaxLobbyNumber) + " Lobbies");
    ClientApp::GetInstance().Send(j.dump());
}

bool LobbyState::IsLobbyFull(Lobby& lb)
{
    return lb.PlayerCount == lb.MaxPlayerCount;
}
