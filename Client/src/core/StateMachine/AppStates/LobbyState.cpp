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
    m_IsTryingToJoinLobby = false;
    m_IsLobbyInit = false;

    Message<MsgType::FetchLobbyList> message;
    ClientConnectionHandler::GetInstance().SendDataToServer(message.Serialize().dump());

    m_ReturnButton = new ButtonComponent(sf::Vector2f(500, 500), sf::Vector2f(200, 100), sf::Color::Red);
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

    m_ReturnButton->Update(dt);
}

void LobbyState::OnExit()
{
    m_Window->ClearAllDrawables();
    m_LobbyButtons.clear();
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
            float x = (lobby.GameMode == GameModeType::CLASSIC) ? 300.0f : 650.0f;
            float y = (i % 3) * 110.0f + 100.0f;
            sf::Color color = (lobby.GameMode == GameModeType::CLASSIC) ? sf::Color(1, 215, 88) : sf::Color(255, 0, 0);
            std::string lobbyName = (lobby.GameMode == GameModeType::CLASSIC) ? "Normal " : "Fast ";

            int playerCount = 0;
            if (!lobby.PlayerX.empty()) playerCount++;
            if (!lobby.PlayerO.empty()) playerCount++;

            if (m_IsLobbyInit)
            {
                m_Lobbies[i].ID = id;
                m_Lobbies[i].GameMode = lobby.GameMode;
                m_Lobbies[i].PlayerO = lobby.PlayerO;
                m_Lobbies[i].PlayerX = lobby.PlayerX;

                m_LobbyButtons[i]->SetButtonText(
                    lobbyName + std::to_string(id) + "\n" + std::to_string(playerCount) + "/" + "2"
                    , sf::Color::White, 30
                    , TextAlignment::Center);

                m_LobbyButtons[i]->SetOnClickCallback([=]()
                {
                    JoinLobbyRequest(i);
                });
            }
            else
            {
                m_Lobbies.emplace_back(id, lobby.GameMode, "", "");

                auto* m_LobbyButton = new ButtonComponent(sf::Vector2f(x, y), sf::Vector2f(200, 100), color);
                m_LobbyButton->SetButtonText(
                    lobbyName + std::to_string(id) + "\n" + std::to_string(playerCount) + "/" + "2"
                    , sf::Color::White, 30
                    , TextAlignment::Center);
                m_LobbyButton->SetOnClickCallback([=]()
                {
                    JoinLobbyRequest(i);
                });

                m_LobbyButtons.push_back(m_LobbyButton);
                m_Window->RegisterDrawable(m_LobbyButton);
            }

            i++;
        }
        m_IsLobbyInit = true;

        break;
    }
    case AcceptJoinLobby:
    {
        ((GameState*)m_StateMachine->GetState("GameState"))->SetLobbyID(m_CurrentLobbyID);
        ((GameState*)m_StateMachine->GetState("GameState"))->SetGameMode(m_LobbyGameMode);
        m_StateMachine->SwitchState("GameState");
        break;
    }
    case RejectJoinLobby:
    {
        m_IsTryingToJoinLobby = false;
        DebugLog("Join lobby rejected");
        break;
    }
    default:
        break;
    }
}

void LobbyState::JoinLobbyRequest(int lobbyID)
{
    if (m_IsTryingToJoinLobby) return;

    m_IsTryingToJoinLobby = true;
    m_CurrentLobbyID = m_Lobbies[lobbyID].ID;
    m_LobbyGameMode = m_Lobbies[lobbyID].GameMode == CLASSIC ? "GameMode: Classic" : "GameMode: Fast";
    Message<MsgType::TryToJoinLobby> message;
    message.LobbyId = m_CurrentLobbyID;
    
    ClientConnectionHandler::GetInstance().SendDataToServer(message.Serialize().dump());
}
