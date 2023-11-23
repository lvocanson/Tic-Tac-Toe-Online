#include "LobbyState.h"
#include "src/core/ClientApp.h"
#include "GameState.h"

#include "tcp-ip/ClientMessages.h"
#include "tcp-ip/ServerMessages.h"

LobbyState::LobbyState(StateMachine* stateMachine, Window* window)
    : State(stateMachine)
    , m_Window(window)
{
    m_StateMachine = stateMachine;
}

LobbyState::~LobbyState()
{
    NULLPTR(m_Window);
}

void LobbyState::OnEnter()
{
    m_IsLobbyInit = false;

    Message<MsgType::Login> message(ClientApp::GetInstance().GetCurrentPlayer()->GetName());

    // RegisterPlayerMessage message(ClientApp::GetInstance().GetCurrentPlayer()->GetName());
    ClientConnectionHandler::GetInstance().SendDataToServer(message);
    // 
    // GetLobbyListNotification message2;
    // ClientConnectionHandler::GetInstance().SendDataToServer(message2.Serialize().dump());

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

    if (m_LeaveButtons)
    {
        m_LeaveButtons->Update(dt);
    }
    if (!m_IsInLobby)
    {
        m_Window->UnregisterDrawable(m_LeaveButtons);
        RELEASE(m_LeaveButtons);
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

    if (m_LeaveButtons != nullptr)
    {
        m_Window->UnregisterDrawable(m_LeaveButtons);
        RELEASE(m_LeaveButtons);
    }

    m_LobbyButtons.clear();

    m_Window->UnregisterDrawable(m_ReturnButton);
    RELEASE(m_ReturnButton);
}

void LobbyState::OnReceiveData(const Json& serializeData)
{
    // if (serializeData["Type"] == "Lobby")
    // {
    //     GetLobbyListMessage message;
    //     message.Deserialize(serializeData);

    //     int i = 0;
    //     for (const auto& lobbyJson : message.AllLobbiesData)
    //     {
    //         int id = lobbyJson.ID;

    //         if (!m_IsLobbyInit)
    //         {
    //             auto* m_LobbyButton = new ButtonComponent(sf::Vector2f(100, i * static_cast<float>(110)), sf::Vector2f(200, 100), sf::Color::Blue);
    //             m_LobbyButton->SetButtonText("Lobby " + std::to_string(id), sf::Color::White, 30, TextAlignment::Center);
    //             m_LobbyButton->SetOnClickCallback([=]()
    //             {
    //                 TryToJoinLobby(i);
    //             });

    //             m_Lobbies.emplace_back(id, "", "");
    //             m_LobbyButtons.push_back(m_LobbyButton);
    //             m_Window->RegisterDrawable(m_LobbyButton);
    //         }
    //         else
    //         {
    //             m_Lobbies[i].ID = id;
    //             m_Lobbies[i].PlayerO = lobbyJson.PlayerO;
    //             m_Lobbies[i].PlayerX = lobbyJson.PlayerX;
    //         }
    //         i++;
    //     }
    //     m_IsLobbyInit = true;
    // }
    // else if (serializeData["Type"] == "JoinedLobby")
    // {
    //     JoinedLobbyMessage message;
    //     message.Deserialize(serializeData);

    //     ((GameState*)m_StateMachine->GetState("GameState"))->SetLobbyID(message.ID);
    //     m_StateMachine->SwitchState("GameState");
    // }
}

void LobbyState::TryToJoinLobby(int lobbyID)
{
    // TryToJoinLobbyMessage message(m_Lobbies[lobbyID].ID);

    // ClientConnectionHandler::GetInstance().SendDataToServer(message.Serialize().dump());
    // m_CurrentLobbyID = m_Lobbies[lobbyID].ID;
}
