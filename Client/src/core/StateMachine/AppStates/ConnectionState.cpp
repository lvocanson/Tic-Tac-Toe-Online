#include "ConnectionState.h"
#include "src/core/ClientApp.h"
#include "tcp-ip/ClientMessages.h"

#include <regex>
#if defined(DEBUG) | defined(_DEBUG)
    #include <SFML/Network/IpAddress.hpp>
#endif


constexpr float CONNECTION_TIMEOUT_TIME = 5.0f;

ConnectionState::ConnectionState(StateMachine* stateMachine, Window* window)
    : State(stateMachine)
    , m_Window(window)
{
    m_StateMachine = stateMachine;
}

ConnectionState::~ConnectionState()
{
    NULLPTR(m_Window);
}

void ConnectionState::OnEnter()
{
    m_IpField = new InsertFieldComponent();
    m_IpField->SetPosition(sf::Vector2f(m_Window->GetWidth() * 0.5f - 190, 100));
    m_IpField->SetLabel("Server Phrase");

#if defined(DEBUG) | defined(_DEBUG)
    m_IpField->SetText(TcpIp::IpAddress::FromString(sf::IpAddress::getLocalAddress().toString()).ToPhrase());
#endif

    m_NameField = new InsertFieldComponent();
    m_NameField->SetPosition(sf::Vector2f(m_Window->GetWidth() * 0.5f - 190, 200));
    m_NameField->SetLabel("Username");

    sf::Color OrangeRed(231, 62, 1);
    m_BackButton = new ButtonComponent(sf::Vector2f(m_Window->GetWidth() * 0.5f - 150, 500), sf::Vector2f(200, 100), OrangeRed);
    m_BackButton->SetButtonText("Return to menu", sf::Color::White, 50, TextAlignment::Center);
    m_BackButton->SetOnClickCallback([this]()
        {
            m_StateMachine->SwitchState("MenuState");
        });

    sf::Color Emerald(1, 215, 88);
    m_ConnectButton = new ButtonComponent(sf::Vector2f(m_Window->GetWidth() * 0.5f - 150, 300), sf::Vector2f(200, 100), Emerald);
    m_ConnectButton->SetButtonText("Connect", sf::Color::White, 50, TextAlignment::Center);
    m_ConnectButton->SetOnClickCallback([this]()
        {
            if (m_IsTryingToConnect) return;

            m_IpField->ClearErrorMessage();
            m_NameField->ClearErrorMessage();

            bool isNameValid = false;
            if (m_NameField->GetText().empty())
            {
                DebugLog("Username should not be empty!\n");
                m_NameField->ShowErrorMessage("Username should not be empty!");
            }
            else if (m_NameField->GetText().size() < 3)
            {
                DebugLog("Username should be more than 2 characters!\n");
                m_NameField->ShowErrorMessage("Username should be more than 2 characters!");
            }
            else isNameValid = true;

            m_Name = m_NameField->GetText();

            const std::string ip = std::string(TcpIp::IpAddress::FromPhrase(m_IpField->GetText()).ToString());

            DebugLog(ip);

            if (isNameValid)
            {
                ClientConnectionHandler::GetInstance().TryToConnectToServer(&ip);
                m_IsTryingToConnect = true;
            }
            else
            {
                DebugLog("Invalid phrase!\n");
                m_IpField->ShowErrorMessage("Invalid phrase!");
            }
        });

    m_Window->RegisterDrawable(m_IpField);
    m_Window->RegisterDrawable(m_NameField);
    m_Window->RegisterDrawable(m_BackButton);
    m_Window->RegisterDrawable(m_ConnectButton);
}

void ConnectionState::OnUpdate(float dt)
{
    m_IpField->Update(dt);
    m_NameField->Update(dt);
    m_BackButton->Update(dt);
    m_ConnectButton->Update(dt);


    if (m_IsTryingToConnect)
    {
        static float timeOutTimer;

        Shared<ConnectionStateInfo>& connectionInfo = ClientConnectionHandler::GetInstance().GetConnectionInfo();

        switch (connectionInfo.WaitGet().Get())
        {
        case Connecting:
        {
            if (timeOutTimer > CONNECTION_TIMEOUT_TIME)
            {
                m_IpField->ShowErrorMessage("Connection timed out!");
                m_IsTryingToConnect = false;
                timeOutTimer = 0.0f;
            }
            else
            {
                timeOutTimer += dt;
            }
            break;
        }
        case Failed:
        {
            m_IpField->ShowErrorMessage("Connection failed!");
            m_IsTryingToConnect = false;
            timeOutTimer = 0.0f;
            break;
        }
        case Connected:
        {
            ClientApp::GetInstance().GetCurrentPlayer()->SetName(m_NameField->GetText());

            m_StateMachine->SwitchState("LobbyState");
            m_IpField->ClearErrorMessage();
            m_IsTryingToConnect = false;

            Message<MsgType::Login> message;
            message.Username = m_Name;
            ClientConnectionHandler::GetInstance().SendDataToServer(message);

            timeOutTimer = 0.0f;
            break;
        }
        default: 
            break;
        }
    }
}

void ConnectionState::OnExit()
{
    m_Window->UnregisterDrawable(m_IpField);
    m_Window->UnregisterDrawable(m_BackButton);
    m_Window->UnregisterDrawable(m_ConnectButton);
    m_Window->UnregisterDrawable(m_NameField);

    RELEASE(m_IpField);
    RELEASE(m_NameField);
    RELEASE(m_ConnectButton);
    RELEASE(m_BackButton);
}

bool ConnectionState::IsValidIpAddress(const char* ip)
{
    const std::regex ipRegex(R"(^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$)");
    return std::regex_match(ip, ipRegex) || std::string("LOCALHOST")._Equal(ip);
}
