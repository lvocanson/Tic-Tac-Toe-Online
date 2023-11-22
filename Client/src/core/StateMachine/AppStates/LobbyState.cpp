#include "LobbyState.h"
#include "src/core/ClientApp.h"
#include "game/IDGenerator.h"
#include "GameState.h"

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
    m_IsLobbyInit = false;

    Json j;
    j["Type"] = "Login";
    j["UserName"] = ClientApp::GetInstance().GetCurrentPlayer()->GetName();
    ClientConnectionHandler::GetInstance().SendDataToServer(j.dump());

    DebugLog(ClientApp::GetInstance().GetCurrentPlayer()->GetName() + "\n");

    j = Json();
    j["Type"] = "GetLobbyList";
    ClientConnectionHandler::GetInstance().SendDataToServer(j.dump());

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
    if (serializeData["Type"] == "Lobby")
    {
        int i = 0;
        for (const auto& lobbyJson : serializeData["Lobbies"])
        {
            int id = lobbyJson["ID"];

            if (!m_IsLobbyInit)
            {
                ButtonComponent* m_LobbyButton = new ButtonComponent(sf::Vector2f(100, (i * 110)), sf::Vector2f(200, 100), sf::Color::Blue);
                m_LobbyButton->SetButtonText("Lobby " + std::to_string(id), sf::Color::White, 30, TextAlignment::Center);
                m_LobbyButton->SetOnClickCallback([=]()
                {
                    if(!m_IsInLobby)
                        TryToJoinLobby(i);
                });

                m_Lobbies.emplace_back(id, "", "");
                m_LobbyButtons.push_back(m_LobbyButton);
                m_Window->RegisterDrawable(m_LobbyButton);
            }
            else 
            {
                std::string playerX = lobbyJson["PlayerX"];
                std::string playerO = lobbyJson["PlayerO"];

                m_Lobbies[i].ID = id;
                m_Lobbies[i].PlayerO = playerO;
                m_Lobbies[i].PlayerX = playerX;
            }
            i++;
        }
        m_IsLobbyInit = true;
    }
    else if (serializeData["Type"] == "IsInLobby")
    {
        Json j;
        j["Type"] = "WaitInLobby";
        j["StartedLobbyID"] = m_CurrentLobbyID;
        ClientConnectionHandler::GetInstance().SendDataToServer(j.dump());

        ((GameState*)m_StateMachine->GetState("GameState"))->SetLobbyID(m_CurrentLobbyID);
        m_StateMachine->SwitchState("GameState");
    }
}

void LobbyState::TryToJoinLobby(int lobbyID)
{
    Json j;
    j["Type"] = "JoinLobby";
    j["ID"] = m_Lobbies[lobbyID].ID;
    ClientConnectionHandler::GetInstance().SendDataToServer(j.dump());
    m_CurrentLobbyID = m_Lobbies[lobbyID].ID;
}
