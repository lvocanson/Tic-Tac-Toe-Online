#include "LobbyState.h"
#include "src/core/ClientApp.h"
#include "game/IDGenerator.h"

LobbyState::LobbyState(StateMachine* stateMachine, Window* window) 
    : State(stateMachine)
    , m_Window(window)
    , m_ReturnButton(nullptr)
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

    //TODO: Define constexpr int MAX_LOBBIES_NUMBER = number;
    for (int i = 0; i < 3; i++)
    {
        ButtonComponent* m_LobbyButton = new ButtonComponent(sf::Vector2f(100, (i*110)), sf::Vector2f(200, 100), sf::Color::Blue);
        m_LobbyButton->SetButtonText("Lobby " + std::to_string(i), sf::Color::White, 30, TextAlignment::Center);
        m_LobbyButton->SetOnClickCallback([=]()
            {
                TryToJoinLobby(i);
            });
        //CreateLeaveLobbyButton(sf::Vector2f(300, (i * 110)), i);
        m_LobbyButtons.push_back(m_LobbyButton);
        m_Window->RegisterDrawable(m_LobbyButton);
    }
  
    
    m_ReturnButton = new ButtonComponent(sf::Vector2f(100, 500), sf::Vector2f(200, 100), sf::Color::Red);
    m_ReturnButton->SetButtonText("Return To Menu", sf::Color::White, 30, TextAlignment::Center);
    m_ReturnButton->SetOnClickCallback([this]()
        {
            m_StateMachine->SwitchState("MenuState");
        });

    m_Window->RegisterDrawable(m_ReturnButton);
}

void LobbyState::OnUpdate(float dt)
{
    for (const auto& lbButton : m_LobbyButtons)
    {
        lbButton->Update(dt);
    }
    m_ReturnButton->Update(dt);
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

void LobbyState::OnReceiveData(const Json& serializeData)
{
    for (const auto& lobbyJson : serializeData["Lobbies"])
    {
        int id = lobbyJson["ID"];
        std::string playerX = lobbyJson["PlayerX"];
        std::string playerO = lobbyJson["PlayerO"];
        m_Lobbies.emplace_back(id, playerX,playerO);
    }
}

void LobbyState::TryToJoinLobby(int lobbyID)
{
    Json j;
    j["Type"] = "JoinLobby";
    j["ID"] = m_Lobbies[lobbyID].ID;
    ClientApp::GetInstance().Send(j.dump());
}

void LobbyState::LeaveLobby(int lobbyID)
{
    Json j;
    j["Type"] = "LeaveLobby";
    j["ID"] = m_Lobbies[lobbyID].ID;
    ClientApp::GetInstance().Send(j.dump());
}

void LobbyState::CreateLeaveLobbyButton(sf::Vector2f pos, int lobbyID)
{
    ButtonComponent* leavebutton = new ButtonComponent(pos, sf::Vector2f(200, 100), sf::Color::Red);
    leavebutton->SetButtonText("Leave Lobby", sf::Color::White, 30, TextAlignment::Center);
    leavebutton->SetOnClickCallback([=]()
        {
            LeaveLobby(lobbyID);
        });

}
