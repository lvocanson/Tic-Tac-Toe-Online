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

    Message<MsgType::FetchLobbyList> message;
    ClientConnectionHandler::GetInstance().SendDataToServer(message.Serialize().dump());

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
    auto type = Message<>::GetType(serializeData);

    using enum MsgType;
    switch (type)
    {
    case LobbyList:
    {
        Message<LobbyList> lobbyList(serializeData);

        int i = 0;
        for (const auto& lobby : lobbyList.LobbiesData)
        {
            int id = lobby.ID;

            if (!m_IsLobbyInit)
            {
                auto* m_LobbyButton = new ButtonComponent(sf::Vector2f(100, i * static_cast<float>(110)), sf::Vector2f(200, 100), sf::Color::Blue);
                m_LobbyButton->SetButtonText("Lobby " + std::to_string(id), sf::Color::White, 30, TextAlignment::Center);
                m_LobbyButton->SetOnClickCallback([=]()
                    {
                        TryToJoinLobby(i);
                    });

                m_Lobbies.emplace_back(id, "", "");
                m_LobbyButtons.push_back(m_LobbyButton);
                m_Window->RegisterDrawable(m_LobbyButton);
            }
            else
            {
                m_Lobbies[i].ID = id;
                m_Lobbies[i].PlayerO = lobby.PlayerO;
                m_Lobbies[i].PlayerX = lobby.PlayerX;
            }
            i++;
        }
        m_IsLobbyInit = true;

        break;
    }
    case AcceptJoinLobby:
    {
        ((GameState*)m_StateMachine->GetState("GameState"))->SetLobbyID(m_CurrentLobbyID);
        m_StateMachine->SwitchState("GameState");
        break;
    }
    case RejectJoinLobby:
    {
        DebugLog("Join lobby rejected");
        break;
    }
    default:
        break;
    }
}

void LobbyState::TryToJoinLobby(int lobbyID)
{
    Message<MsgType::JoinLobby> message;
    message.LobbyId = static_cast<unsigned int>(lobbyID);
    
    ClientConnectionHandler::GetInstance().SendDataToServer(message.Serialize().dump());
    m_CurrentLobbyID = m_Lobbies[lobbyID].ID;
}
