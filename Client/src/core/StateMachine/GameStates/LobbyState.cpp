#include "LobbyState.h"
#include "../Client/src/core/ClientApp.h"
#include "../TicTacToe/IDGenerator.h"

LobbyState::LobbyState(StateMachine* stateMachine, Window* window) 
    : State(stateMachine)
    , m_Window(window)
    , m_ReturnButton(nullptr)
    , m_LobbyButton(nullptr)
    , m_GameStateUI(nullptr)
    , m_Lobbies()
{
    m_StateMachine = stateMachine;
}

LobbyState::~LobbyState()
{
    NULLPTR(m_Window);
}


void LobbyState::OnEnter()
{
    Json j;
    j["Type"] = "LobbyList";
    ClientApp::GetInstance().Send(j.dump());

    m_LobbyButton = new ButtonComponent(sf::Vector2f(100, 100), sf::Vector2f(200, 100), sf::Color::Blue);
    m_LobbyButton->SetButtonText("Lobby 1", sf::Color::White, 30, TextAlignment::Center);
    m_LobbyButton->SetOnClickCallback([&]()
        {
            JoinLobby();
        });
    m_Window->RegisterDrawable(m_LobbyButton);
    
    m_ReturnButton = new ButtonComponent(sf::Vector2f(100, 500), sf::Vector2f(200, 100), sf::Color::Red);
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
    m_LobbyButton->Update(dt);
    m_ReturnButton->Update(dt);
}

void LobbyState::OnExit()
{
    m_Window->UnregisterDrawable(m_LobbyButton);
    RELEASE(m_LobbyButton);

    m_Window->UnregisterDrawable(m_ReturnButton);
    RELEASE(m_ReturnButton);
}

void LobbyState::OnReceiveData(const Json& serializeData)
{
    for (const auto& lobbyJson : serializeData["Lobbies"])
    {
        Lobby lb;
        int id = lobbyJson["ID"];
        std::string playerX = lobbyJson["PlayerX"];
        std::string playerO = lobbyJson["PlayerO"];
        lb.ID = id;
        lb.PlayerX = playerX;
        lb.PlayerO = playerO;
        m_Lobbies.push_back(lb);
    }
    std::cout << m_Lobbies.size();
}


void LobbyState::JoinLobby()
{
    Json j;
    j["Type"] = "JoinLobby";
    ClientApp::GetInstance().Send(j.dump());
}

void LobbyState::LeaveLobby()
{

}